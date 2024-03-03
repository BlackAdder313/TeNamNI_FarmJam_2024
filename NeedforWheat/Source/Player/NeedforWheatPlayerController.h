// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NeedforWheatPlayerController.generated.h"

class UInputMappingContext;
class ANeedforWheatPawn;
class UNeedforWheatUI;
class ANFWFarmingAreaTrigger;
class ANeedforWheatGameMode;

/**
 *  Vehicle Player Controller class
 *  Handles input mapping and user interface
 */
UCLASS(abstract)
class NEEDFORWHEAT_API ANeedforWheatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void RegisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea);
	void UnregisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea);

	void TryStartWheatCollection();
	void FinishLevel(int32 score);

protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	TObjectPtr<ANeedforWheatPawn> VehiclePawn;

	/** Type of the UI to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UNeedforWheatUI> VehicleUIClass;

	/** Pointer to the UI widget */
	TObjectPtr<UNeedforWheatUI> VehicleUI;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ANeedforWheatPawn> HarvestingVehicle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ANeedforWheatPawn> PlantingVehicle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float LevelTimerInSeconds = 600;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int WheatPointsValue = 10;

protected:
	// Actor interface overrides
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;

	// PlayerController interface overrides
	virtual void OnPossess(APawn* InPawn) override;

private:
	TWeakObjectPtr<ANFWFarmingAreaTrigger> m_farmingArea;
	TArray<FVector> m_positionsInFarmingArea;
	TWeakObjectPtr<ANeedforWheatGameMode> m_gameMode;

	float m_levelTimer = 0;
	bool m_wheatCollectionStarted = false;
};
