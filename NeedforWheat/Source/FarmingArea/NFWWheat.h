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

	void BeginPlay() override;

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintNativeEvent, Category = Wheat)
	void OnWheatCollectionStart();

protected:
	UFUNCTION()
	void OnCollectByVehicle(AActor* OverlappedActor, AActor* OtherActor);
};
