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
		if (m_farmingArea.IsValid())
		{
			m_positionsInFarmingArea.Add(GetPawn()->GetActorLocation());

			if (timer > .75f && m_gameMode.IsValid())
			{
				m_farmingArea->UpdateVehiclePositions(m_positionsInFarmingArea);
				m_positionsInFarmingArea.Empty();
				
				auto [plantedWheat, totalWheat] = m_gameMode.Get()->GetFarmingAreasWheatInfo();
				VehicleUI->UpdatePlantedWheat(plantedWheat, totalWheat);
				timer = 0.f;

				// Temp debug code
				// Will be replaced with button press
				if (2 * plantedWheat >= totalWheat)
				{
					TArray<AActor*> matchingActors;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANFWWheat::StaticClass(), matchingActors);

					for (const auto& wheat : matchingActors)
					{
						Cast<ANFWWheat>(wheat)->OnWheatCollectionStart();
					}
				}
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
