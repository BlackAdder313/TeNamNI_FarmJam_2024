// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NeedforWheatUI.generated.h"

/**
 *  Simple Vehicle HUD class
 *  Displays the current speed and gear.
 *  Widget setup is handled in a Blueprint subclass.
 */
UCLASS(abstract)
class NEEDFORWHEAT_API UNeedforWheatUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** Controls the display of speed in Km/h or MPH */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

public:

	/** Called to update the speed display */
	void UpdateSpeed(float NewSpeed);

	/** Called to update the gear display */
	void UpdateGear(int32 NewGear);

	/** Called to update the planted wheat display */
	void UpdatePlantedWheat(int32 PlantedWheat, int32 TotalWheat);

	/** Called to update the planted wheat display */
	void UpdateCollectedWheat(int32 PlantedWheat, int32 TotalWheat);
	
	void CanStartWheatCollection();

	void UpdateLevelTimer(float LevelTimer);
	void UpdateLevelScore(int32 score);

	void SetWheatCollectionEnabled(bool isWheatCollectionEnabled);

	void FinishLevel(int32 score, int32 highScore);

protected:

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnSpeedUpdate(float NewSpeed);

	/** Implemented in Blueprint to display the new gear */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnGearUpdate(int32 NewGear);

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnPlantedWheatAmountUpdate(int32 PlantedWheat, int32 TotalWheat);

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnCollectedWheatAmountUpdate(int32 CollectedWheat, int32 TotalWheat);

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnLevelTimerUpdate(int32 LevelTimer);

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnLevelScoreUpdate(int32 score);

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnLevelFinished(int32 score, int32 highScore);

	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnCanStartWheatCollection();

	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnSetWheatCollectionEnabled(bool isWheatCollectionEnabled);
};
