// Copyright Epic Games, Inc. All Rights Reserved.


#include "NeedforWheatPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NeedforWheatGameMode.h"
#include "NeedforWheatPawn.h"
#include "FarmingArea\NFWFarmingAreaTrigger.h"
#include "FarmingArea\NFWWheat.h"
#include "UI\NeedforWheatUI.h"

void ANeedforWheatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UNeedforWheatUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	m_gameMode = MakeWeakObjectPtr<ANeedforWheatGameMode>(Cast<ANeedforWheatGameMode>(GetWorld()->GetAuthGameMode()));
}

void ANeedforWheatPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	static float timer = 0.f;
	timer += Delta;

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
		if (m_farmingArea.IsValid() && m_gameMode.IsValid() && m_gameMode.Get()->GetFarmingStatus() == EFarmingStatus::Plant)
		{
			m_positionsInFarmingArea.Add(GetPawn()->GetActorLocation());

			if (timer > .75f )
			{
				m_farmingArea->UpdateVehiclePositions(m_positionsInFarmingArea);
				m_positionsInFarmingArea.Empty();
				
				auto [plantedWheat, totalWheat] = m_gameMode.Get()->GetFarmingAreasWheatInfo();
				VehicleUI->UpdatePlantedWheat(plantedWheat, totalWheat);
				timer = 0.f;
			}
		}
	}
}

void ANeedforWheatPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<ANeedforWheatPawn>(InPawn);
}


void ANeedforWheatPlayerController::RegisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea)
{
	if (!m_farmingArea.IsValid())
	{
		m_farmingArea = farmingArea;
	}
}

void ANeedforWheatPlayerController::UnregisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea)
{
	if (m_farmingArea.IsValid() && m_farmingArea.Get() == farmingArea)
	{
		m_positionsInFarmingArea.Empty();
		m_farmingArea.Reset();
	}
}

void ANeedforWheatPlayerController::TryStartWheatCollection()
{
	if (!m_gameMode.IsValid())
	{
		return;
	}

	if (m_gameMode->TryStartWheatCollection())
	{
		if (m_gameMode->GetFarmingStatus() == EFarmingStatus::Collect)
		{
			TArray<AActor*> matchingActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANFWWheat::StaticClass(), matchingActors);

			for (const auto& wheat : matchingActors)
			{
				Cast<ANFWWheat>(wheat)->OnWheatCollectionStart();
			}

			TArray<AActor*> vehicles;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), HarvestingVehicle, vehicles);

			if (vehicles.IsEmpty())
			{
				return;
			}

			if (ANeedforWheatPawn* harvestingVehicle = Cast<ANeedforWheatPawn>(vehicles[0]))
			{
				Possess(harvestingVehicle);
			}

			for (const auto& wheat : matchingActors)
			{
				Cast<ANFWWheat>(wheat)->OnWheatCollectionStart();
			}

			return;
		}

		TArray<AActor*> vehicles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlantingVehicle, vehicles);

		if (vehicles.IsEmpty())
		{
			return;
		}

		if (ANeedforWheatPawn* harvestingVehicle = Cast<ANeedforWheatPawn>(vehicles[0]))
		{
			Possess(harvestingVehicle);
		}
	}
}
