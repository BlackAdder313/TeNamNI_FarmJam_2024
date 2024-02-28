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