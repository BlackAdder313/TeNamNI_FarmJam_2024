// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatGameMode.h"

#include "FarmingArea\NFWFarmingAreaTrigger.h"
#include "Player\NeedforWheatPlayerController.h"

static float s_startWheatCollectionThreshold = 0.5f;

ANeedforWheatGameMode::ANeedforWheatGameMode()
{
	PlayerControllerClass = ANeedforWheatPlayerController::StaticClass();
}

void ANeedforWheatGameMode::PostInitProperties()
{
	Super::PostInitProperties();
	m_registeredFarmingAreas.Empty();

	m_farmingStatus = EFarmingStatus::Plant;
	m_collectedWheatInfo = { 0 , 1 };
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

bool ANeedforWheatGameMode::TryStartWheatCollection()
{
	if (m_farmingStatus == EFarmingStatus::Collect)
	{
		m_farmingStatus = EFarmingStatus::Plant;
		return true;
	}

	auto [plantedWheat, totalWheat] = GetFarmingAreasWheatInfo();

	if (CanStartWheatCollection())
	{
		BeginWheatCollection();
		return true;
	}
	
	return false;
}

bool ANeedforWheatGameMode::CanStartWheatCollection() const
{
	auto [plantedWheat, totalWheat] = GetFarmingAreasWheatInfo();

	if (static_cast<float>(plantedWheat) / static_cast<float>(totalWheat) < s_startWheatCollectionThreshold)
	{
		return false;
	}

	return true;
}

void ANeedforWheatGameMode::BeginWheatCollection()
{
	m_farmingStatus = EFarmingStatus::Collect;

	for (const auto& farmingArea : m_registeredFarmingAreas)
	{
		if (!farmingArea.IsValid())
		{
			continue;
		}

		auto [plantedWheat, totalWheat] = farmingArea->GetPlantedWheatInfo();
		m_collectedWheatInfo.Value += totalWheat;
	}
}

void ANeedforWheatGameMode::OnWheatCollected()
{
	m_collectedWheatInfo.Key++;
}