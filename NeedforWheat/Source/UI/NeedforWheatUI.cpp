// Copyright Epic Games, Inc. All Rights Reserved.


#include "NeedforWheatUI.h"

void UNeedforWheatUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UNeedforWheatUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UNeedforWheatUI::UpdatePlantedWheat(int32 PlantedWheat, int32 TotalWheat)
{
	// call the Blueprint handler
	OnPlantedWheatAmountUpdate(PlantedWheat, TotalWheat);
}

void UNeedforWheatUI::UpdateCollectedWheat(int32 CollectedWheat, int32 TotalWheat)
{
	// call the Blueprint handler
	OnCollectedWheatAmountUpdate(CollectedWheat, TotalWheat);
}

void UNeedforWheatUI::UpdateLevelTimer(float LevelTimer)
{
	// call the Blueprint handler
	int32 fixedLevelTimer = FMath::Max(0, static_cast<int>(LevelTimer));
	OnLevelTimerUpdate(fixedLevelTimer);
}

void UNeedforWheatUI::UpdateLevelScore(int32 score)
{
	OnLevelScoreUpdate(score);
}

void UNeedforWheatUI::CanStartWheatCollection()
{
	OnCanStartWheatCollection();
}

void UNeedforWheatUI::SetWheatCollectionEnabled(bool isWheatCollectionEnabled)
{
	OnSetWheatCollectionEnabled(isWheatCollectionEnabled);
}