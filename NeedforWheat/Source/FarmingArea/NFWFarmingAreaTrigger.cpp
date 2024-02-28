// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWFarmingAreaTrigger.h"

#include "Kismet/KismetMathLibrary.h"

#include "NeedforWheatGameMode.h"
#include "Player\NeedforWheatPawn.h"
#include "Player\NeedforWheatPlayerController.h"

namespace Internal
{
	const int wheatPositionSortingRadius = 10;

	bool IsPointWithinArea(const FVector& point, const FVector& areaCenter, float areaHeight, float areaWidth, float offset = 0.f)
	{
		return FMath::IsWithin(point.X, areaCenter.X - areaHeight - offset, areaCenter.X + areaHeight + offset)
			&& FMath::IsWithin(point.Y, areaCenter.Y - areaWidth - offset, areaCenter.Y + areaWidth + offset);
	}
}

void ANFWFarmingAreaTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	OnActorBeginOverlap.AddDynamic(this, &ANFWFarmingAreaTrigger::OnPlayerEnterTriggerArea);
	OnActorEndOverlap.AddDynamic(this, &ANFWFarmingAreaTrigger::OnPlayerExitTriggerArea);
}

void ANFWFarmingAreaTrigger::OnPlayerEnterTriggerArea(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const ANeedforWheatPawn* pawnActor = Cast<ANeedforWheatPawn>(OtherActor))
	{
		pawnActor->GetController<ANeedforWheatPlayerController>()->RegisterFarmingArea(this);
		UE_LOG(LogTemp, Warning, TEXT("Enter Farming Area"));
		m_actorToIgnore = MakeWeakObjectPtr<AActor>(OtherActor);
	}
}

void ANFWFarmingAreaTrigger::OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const ANeedforWheatPawn* pawnActor = Cast<ANeedforWheatPawn>(OtherActor))
	{
		pawnActor->GetController<ANeedforWheatPlayerController>()->UnregisterFarmingArea(this);
		m_actorToIgnore.Reset();
		
		if (Debug_SpawnWheatOnExitFromFarmingArea)
		{
			for (auto position : m_wheatPositionsToSprout)
			{
				SpawnWheat(position);
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Exit Farming Area"));
	}
}

void ANFWFarmingAreaTrigger::BeginPlay()
{
	Super::BeginPlay();

	GetActorBounds(true, m_origin, m_areaBounds);

	float halfHeight = m_areaBounds.X;
	float halfWidth = m_areaBounds.Y;

	m_verticalWheatHalfAmount = halfHeight / WheatSpacing;
	m_horizontalWheatHalfAmount = halfWidth / WheatSpacing;

	m_wheatPositions.Init(TArray<FVector>(), m_verticalWheatHalfAmount * 2);

	for (uint16_t heightIndex = 0, h_len = m_verticalWheatHalfAmount; heightIndex <= h_len; heightIndex++)
	{
		uint16_t bottomIndex = m_verticalWheatHalfAmount + heightIndex;
		uint16_t topIndex = m_verticalWheatHalfAmount - heightIndex;

		for (uint16_t widthIndex = 0, w_len = m_horizontalWheatHalfAmount; widthIndex <= w_len; widthIndex++)
		{
			bool successInsertOnTopLine = TryInsertWheatPositionsAroundIndex(topIndex, -heightIndex, widthIndex);
			bool successInsertOnBottomLine = TryInsertWheatPositionsAroundIndex(bottomIndex, heightIndex, widthIndex);

			if(!successInsertOnTopLine && !successInsertOnBottomLine)
			{
				break;
			}
		}
	}

	for (auto& positionsList : m_wheatPositions)
	{
		positionsList.Sort([](const FVector& left, const FVector& right) { return left.Y < right.Y; });
	}

	if (Debug_SpawnWheatOnBeginPlay)
	{
		for (auto position : m_wheatPositionsToSprout)
		{
			SpawnWheat(position);
		}
	}

	if (ANeedforWheatGameMode* gameMode = Cast<ANeedforWheatGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->RegisterFarmingArea(this);
	}
}

void ANFWFarmingAreaTrigger::UpdateVehiclePositions(const TArray<FVector>& vehiclePositions)
{
	for (const auto& vehiclePosition : vehiclePositions)
	{
		// Shorten the vertical range of searched positions based on vehicle positions
		TPair<uint16_t, uint16_t> verticalIndices = { 0, m_wheatPositions.Num() - 1 };
		uint16_t verticalIndicesRange = verticalIndices.Value - verticalIndices.Key;
		while (verticalIndicesRange > Internal::wheatPositionSortingRadius)
		{
			uint16_t middleVerticalIndex = (verticalIndices.Value + verticalIndices.Key) / 2;
			auto heightValue = m_wheatPositions[middleVerticalIndex][0].X;
			if (FMath::IsNearlyEqual(vehiclePosition.X, heightValue) || vehiclePosition.X < heightValue)
			{
				verticalIndices = { verticalIndices.Key, middleVerticalIndex };
			}
			else
			{
				verticalIndices = { middleVerticalIndex , verticalIndices.Value };
			}

			verticalIndicesRange = verticalIndices.Value - verticalIndices.Key;
		}
		
		// Shorten the horizontal range of searched positions based on vehicle positions
		for (uint16_t verticalIndex = verticalIndices.Key; verticalIndex <= verticalIndices.Value; verticalIndex++)
		{
			TArray<FVector> positionsList = m_wheatPositions[verticalIndex];
			TPair<uint16_t, uint16_t> horizontalIndices = { 0, positionsList.Num() };
			uint16_t horizontalIndicesRange = horizontalIndices.Value - horizontalIndices.Key;
			
			while (horizontalIndicesRange > Internal::wheatPositionSortingRadius)
			{
				uint16_t middleHorizontalIndex = (horizontalIndices.Value + horizontalIndices.Key) / 2;
				auto widthValue = positionsList[middleHorizontalIndex].Y;

				if (FMath::IsNearlyEqual(vehiclePosition.Y, widthValue) || vehiclePosition.Y < widthValue)
				{
					horizontalIndices = { horizontalIndices.Key, middleHorizontalIndex };
				}
				else
				{
					horizontalIndices = { middleHorizontalIndex , horizontalIndices.Value };
				}

				horizontalIndicesRange = horizontalIndices.Value - horizontalIndices.Key;
			}

			// Update positions to sprout
			positionsList.RemoveAtSwap(horizontalIndices.Value, positionsList.Num() - horizontalIndices.Value);
			positionsList.RemoveAtSwap(horizontalIndices.Key);
			UpdateWheatPositionsToSprout(vehiclePosition, positionsList, verticalIndex);
		}
	}
}

bool ANFWFarmingAreaTrigger::TryInsertWheatPositionsAroundIndex(uint16_t index, float heightOffset, float widthOffset)
{
	if(m_wheatPositions.IsValidIndex(index))
	{
		// Move heightIndex lines below origin
		FVector leftPoint = m_origin + FVector(heightOffset * WheatSpacing, -widthOffset * WheatSpacing, 0.f);
		FVector rightPoint = m_origin + FVector(heightOffset * WheatSpacing, widthOffset * WheatSpacing, 0.f);

		m_wheatPositions[index].AddUnique(leftPoint);
		m_wheatPositions[index].AddUnique(rightPoint);

		if (Debug_SpawnWheatOnBeginPlay)
		{
			m_wheatPositionsToSprout.AddUnique(leftPoint);
			m_wheatPositionsToSprout.AddUnique(rightPoint);
		}

		return true;
	}

	return false;
}

void ANFWFarmingAreaTrigger::TrySpreadWheatToSprout(uint16_t verticalIndex, uint16_t horizontalIndex, uint16_t horizontalOffset)
{
	if (m_wheatPositions.IsValidIndex(verticalIndex))
	{
		uint16_t leftIndex = horizontalIndex - horizontalOffset;
		if (m_wheatPositions[verticalIndex].IsValidIndex(leftIndex))
		{
			SpawnWheat(m_wheatPositions[verticalIndex][leftIndex]);
		}

		uint16_t rightIndex = horizontalIndex + horizontalOffset;
		if (m_wheatPositions[verticalIndex].IsValidIndex(rightIndex))
		{
			SpawnWheat(m_wheatPositions[verticalIndex][rightIndex]);
		}
	}
}

void ANFWFarmingAreaTrigger::UpdateWheatPositionsToSprout(const FVector& position,
														  const TArray<FVector>& sproutPositions,
														  const uint16_t verticalIndex)
{
	for (const auto& sproutPosition : sproutPositions)
	{
		if (Internal::IsPointWithinArea(position, sproutPosition, WheatSpacing, WheatSpacing))
		{
			SpawnWheat(sproutPosition);
			uint16_t horizontalIndex = m_wheatPositions[verticalIndex].Find(sproutPosition);

			for (uint16_t verticalOffset = 0; verticalOffset <= WheatSproutRadius; verticalOffset++)
			{
				uint16_t topIndex = verticalIndex - verticalOffset;
				uint16_t bottomIndex = verticalIndex + verticalOffset;
				for (uint16_t horizontalOffset = 1; horizontalOffset <= WheatSproutRadius; horizontalOffset++)
				{
					TrySpreadWheatToSprout(topIndex, horizontalIndex, horizontalOffset);
					TrySpreadWheatToSprout(bottomIndex, horizontalIndex, horizontalOffset);
				}
			}

			break;
		}
	}
}

void ANFWFarmingAreaTrigger::SpawnWheat(FVector position)
{
	size_t newIndex = m_wheatPositionsToSprout.AddUnique(position);
	if (newIndex <  m_wheatPositionsToSprout.Num() - 1)
	{
		// This wheat position has been already spawned
		return;
	}

	FHitResult hitResult;

	FCollisionQueryParams QueryParams;
	if (m_actorToIgnore.IsValid())
	{
		QueryParams.AddIgnoredActor(m_actorToIgnore.Get());
	}

	FVector traceStart = position + FVector(0.f, 0.f, 1000.f);
	FVector traceEnd = position - FVector(0.f, 0.f, 1000.f);

	FRotator wheatNormal;
	bool success = GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_WorldStatic, QueryParams);
	if (success)
	{
		position.Z = hitResult.ImpactPoint.Z;
		wheatNormal = UKismetMathLibrary::MakeRotFromZ(hitResult.ImpactNormal);
	}

	GetWorld()->SpawnActor<AActor>(WheatActor, position, wheatNormal);
	
}

TPair<int, int> ANFWFarmingAreaTrigger::GetPlantedWheatInfo() const
{
	static const int totalWheatNumber = m_verticalWheatHalfAmount * m_horizontalWheatHalfAmount * 4;
	return TPair<int, int>(m_wheatPositionsToSprout.Num(), totalWheatNumber);
}