// Copyright Epic Games, Inc. All Rights Reserved.


#include "NeedforWheatPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "NeedforWheatPawn.h"
#include "FarmingArea\NFWFarmingAreaTrigger.h"
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
}

void ANeedforWheatPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
		if (m_farmingArea.IsValid())
		{
			m_positionsInFarmingArea.Add(GetPawn()->GetTransform().GetLocation());
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
		m_farmingArea->UpdateVehiclePositions(m_positionsInFarmingArea);
		m_positionsInFarmingArea.Empty();
		m_farmingArea.Reset();
	}
}
