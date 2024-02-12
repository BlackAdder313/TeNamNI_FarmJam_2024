// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWFarmingAreaTrigger.h"

#include "Player\NeedforWheatPawn.h"
#include "Player\NeedforWheatPlayerController.h"

// Could be fetched by the BP, but for now it's hardcoded
static const float s_wheatAreaSideLength = 45.f;

namespace Internal
{
	bool IsPointWithinArea(const FVector& point, const FVector& areaCenter, float areaWidth, float areaHeight)
	{
		return FMath::IsWithin(point.X, areaCenter.X - areaWidth * 0.5, areaCenter.X + areaWidth * 0.5)
			&& FMath::IsWithin(point.Y, areaCenter.Y - areaHeight * 0.5, areaCenter.Y + areaHeight * 0.5);
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
		UE_LOG(LogTemp, Warning, TEXT("Entered"));
	}
}

void ANFWFarmingAreaTrigger::OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const ANeedforWheatPawn* pawnActor = Cast<ANeedforWheatPawn>(OtherActor))
	{
		pawnActor->GetController<ANeedforWheatPlayerController>()->UnregisterFarmingArea(this);
		for (const auto& position : m_wheatPositionsToSprout)
		{
			GetWorld()->SpawnActor<AActor>(WheatActor, position, FRotator());
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Left"));
	}
}

void ANFWFarmingAreaTrigger::BeginPlay()
{
	Super::BeginPlay();

	GetActorBounds(true, m_origin, m_areaBounds);

	float width = m_areaBounds.Y;
	float height = m_areaBounds.X;

	m_verticalWheatAmount = height / s_wheatAreaSideLength;
	m_horizontalWheatAmount = width / s_wheatAreaSideLength;

	int maxWheatAmount = static_cast<int>(height / s_wheatAreaSideLength) * static_cast<int>(width / s_wheatAreaSideLength);
	UE_LOG(LogTemp, Warning, TEXT("Max Wheat Amount: %d"), maxWheatAmount);

	for (uint16_t widthIndex = 0, w_len = m_horizontalWheatAmount * .5; widthIndex < w_len; widthIndex++)
	{
		for (uint16_t heightIndex = 0, h_len = m_verticalWheatAmount * .5; heightIndex < h_len; heightIndex++)
		{
			FVector bottomLeftPoint = m_origin - FVector(widthIndex * s_wheatAreaSideLength, heightIndex * s_wheatAreaSideLength, 0.f);
			FVector bottomRightPoint = m_origin + FVector(widthIndex * s_wheatAreaSideLength, -heightIndex * s_wheatAreaSideLength, 0.f);
			
			FVector topLeftPoint = m_origin - FVector(widthIndex * s_wheatAreaSideLength, -heightIndex * s_wheatAreaSideLength, 0.f);
			FVector topRightPoint = m_origin + FVector(widthIndex * s_wheatAreaSideLength, heightIndex * s_wheatAreaSideLength, 0.f);
			
			m_wheatPositions_bottomLeft.Add(bottomLeftPoint);
			m_wheatPositions_bottomRight.Add(bottomRightPoint);
			
			m_wheatPositions_topLeft.Add(topLeftPoint);
			m_wheatPositions_topRight.Add(topRightPoint);

			m_wheatPositionsToSprout.Add(bottomLeftPoint);
			m_wheatPositionsToSprout.Add(bottomRightPoint);

			m_wheatPositionsToSprout.Add(topLeftPoint);
			m_wheatPositionsToSprout.Add(topRightPoint);
		}
	}

	for (const auto& position : m_wheatPositionsToSprout)
	{
		GetWorld()->SpawnActor<AActor>(WheatActor, position, FRotator());
	}
}

void ANFWFarmingAreaTrigger::UpdateVehiclePositions(const TArray<FVector>& vehiclePositions)
{
	float width = m_areaBounds.Y * .5f;
	float height = m_areaBounds.X * .5f;
	for (const auto& vehiclePosition : vehiclePositions)
	{
		// bottom left
		FVector areaOrigin = m_origin - FVector(width, height, 0.f);
		if (Internal::IsPointWithinArea(vehiclePosition, areaOrigin, width, height))
		{
			UpdateWheatPositionsToSprout(vehiclePosition, m_wheatPositions_bottomLeft);
			continue;
		}

		// bottom right
		areaOrigin = m_origin + FVector(width, -height, 0.f);
		if (Internal::IsPointWithinArea(vehiclePosition, areaOrigin, width, height))
		{
			UpdateWheatPositionsToSprout(vehiclePosition, m_wheatPositions_bottomRight);
			continue;
		}

		// top left
		areaOrigin = m_origin - FVector(width, -height, 0.f);
		if (Internal::IsPointWithinArea(vehiclePosition, areaOrigin, width, height))
		{
			UpdateWheatPositionsToSprout(vehiclePosition, m_wheatPositions_topLeft);
			continue;
		}

		areaOrigin = m_origin + FVector(width, height, 0.f);
		if (Internal::IsPointWithinArea(vehiclePosition, areaOrigin, width, height))
		{
			UpdateWheatPositionsToSprout(vehiclePosition, m_wheatPositions_topRight);
			continue;
		}

		UE_LOG(LogTemp, Error, TEXT("No position found: %s,"), *vehiclePosition.ToCompactString());
	}
}

void ANFWFarmingAreaTrigger::UpdateWheatPositionsToSprout(const FVector& position, const TArray<FVector>& sproutPositions)
{
	for (const auto& sproutPosition : sproutPositions)
	{
		if (Internal::IsPointWithinArea(position, sproutPosition, s_wheatAreaSideLength, s_wheatAreaSideLength))
		{
			m_wheatPositionsToSprout.Add(position);
			break;
		}
	}
}