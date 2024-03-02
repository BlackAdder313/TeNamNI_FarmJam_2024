// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWWheat.h"

#include "Kismet/KismetMathLibrary.h"

#include "NeedforWheatGameMode.h"
#include "Player\NeedforWheatPawn.h"

static const float s_SproutScale = .65f;
static const float s_WheatScale = 1.3f;

void ANFWWheat::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SetActorEnableCollision(false);
	m_delayTimer = 0.f;

	m_shouldGrowFullWheat = false;
	SetActorTickEnabled(true);
}

void ANFWWheat::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasActorBegunPlay())
	{
		return;
	}

	GrowSprout(m_shouldGrowFullWheat ? s_WheatScale : s_SproutScale, DeltaSeconds);
}

void ANFWWheat::GrowSprout(float targetScale, float deltaTime)
{
	if (Debug_DisableScaleAnimation)
	{
		SetActorScale3D(FVector(targetScale));
		SetActorTickEnabled(false);
		return;
	}

	if (m_delayTimer > 0.f)
	{
		m_delayTimer -= deltaTime;
		return;
	}

	float scaleOffset = UKismetMathLibrary::RandomFloatInRange(0.01, 0.05);
	SetActorScale3D(GetActorScale3D() + scaleOffset);

	if (GetActorScale3D().GetMin() > targetScale)
	{
		SetActorScale3D(FVector(targetScale));
		SetActorTickEnabled(false);
		m_shouldGrowFullWheat = false;
		m_delayTimer = 0.f;
	}
}

void ANFWWheat::OnWheatCollectionStart_Implementation()
{
	m_shouldGrowFullWheat = true;
	m_delayTimer = UKismetMathLibrary::RandomFloatInRange(0.5, 4.5);
	SetActorTickEnabled(true);

	SetActorEnableCollision(true);
	OnActorBeginOverlap.AddUniqueDynamic(this, &ANFWWheat::OnCollectByVehicle);
}

void ANFWWheat::OnCollectByVehicle(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const ANeedforWheatPawn* pawnActor = Cast<ANeedforWheatPawn>(OtherActor))
	{
		if (auto gameMode = Cast<ANeedforWheatGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (gameMode->GetFarmingStatus() == EFarmingStatus::Collect)
			{
				gameMode->OnWheatCollected();

				SetActorEnableCollision(false);
				SetLifeSpan(UKismetMathLibrary::RandomFloatInRange(0.2, 0.6));
			}
		}
	}
}