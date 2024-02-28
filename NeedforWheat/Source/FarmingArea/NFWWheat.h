// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "NFWWheat.generated.h"

/**
 *  Rear wheel definition for Sports Car.
 */
UCLASS()
class NEEDFORWHEAT_API ANFWWheat : public AActor
{
	GENERATED_BODY()
	
public:
	ANFWWheat() = default;

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnWheatCollectionStart();
};
