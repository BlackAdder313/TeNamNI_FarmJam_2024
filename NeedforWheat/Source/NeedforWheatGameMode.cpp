// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatGameMode.h"

#include "FarmingArea\NFWFarmingAreaTrigger.h"
#include "Player\NeedforWheatPlayerController.h"

ANeedforWheatGameMode::ANeedforWheatGameMode()
{
	PlayerControllerClass = ANeedforWheatPlayerController::StaticClass();
}

void ANeedforWheatGameMode::PostInitProperties()
{
	Super::PostInitProperties();
	m_registeredFarmingAreas.Empty();
}

void ANeedforWheatGameMode::RegisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea)
{
	m_registeredFarmingAreas.AddUnique(MakeWeakObjectPtr(farmingArea));
}

TPair<int, int> ANeedforWheatGameMode::GetFarmingAreasWheatInfo() const
{
	TPair<int, int> wheatInfo;
	for (const auto& farmingArea : m_registeredFarmingAreas)
	{
		if (!farmingArea.IsValid())
		{
			continue;
		}

		auto [plantedWheat, totalWheat] = farmingArea->GetPlantedWheatInfo();
		wheatInfo.Key += plantedWheat;
		wheatInfo.Value += totalWheat;
	}

	return wheatInfo;
}
