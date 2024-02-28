// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeedforWheatGameMode.generated.h"

class ANFWFarmingAreaTrigger;

enum class EFarmingStatus
{
	Plant,
	Collect
};

UCLASS(MinimalAPI)
class ANeedforWheatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeedforWheatGameMode();

	virtual void PostInitProperties() override;

	void RegisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea);
	TPair<int, int> GetFarmingAreasWheatInfo() const;

	void BeginWheatCollection();
	void OnWheatCollected();

	EFarmingStatus GetFarmingStatus() const { return m_farmingStatus; }

	bool TryStartWheatCollection();

private:
	TArray<TWeakObjectPtr<ANFWFarmingAreaTrigger>> m_registeredFarmingAreas;

	EFarmingStatus m_farmingStatus = EFarmingStatus::Plant;
	TPair<int, int> m_collectedWheatInfo;
};



