// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "NFWFarmingAreaTrigger.generated.h"

/**
 *  Rear wheel definition for Sports Car.
 */
UCLASS()
class NEEDFORWHEAT_API ANFWFarmingAreaTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ANFWFarmingAreaTrigger() = default;
	void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnPlayerEnterTriggerArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor);
};
