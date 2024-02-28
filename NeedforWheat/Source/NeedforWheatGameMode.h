// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeedforWheatGameMode.generated.h"

class ANFWFarmingAreaTrigger;

UCLASS(MinimalAPI)
class ANeedforWheatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeedforWheatGameMode();

	virtual void PostInitProperties() override;

	void RegisterFarmingArea(ANFWFarmingAreaTrigger* farmingArea);
	TPair<int, int> GetFarmingAreasWheatInfo() const;

private:
	TArray<TWeakObjectPtr<ANFWFarmingAreaTrigger>> m_registeredFarmingAreas;
};



