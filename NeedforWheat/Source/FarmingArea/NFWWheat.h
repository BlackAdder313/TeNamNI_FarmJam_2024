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

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Implemented in Blueprint to display the planted wheat */
	UFUNCTION(BlueprintNativeEvent, Category = Wheat)
	void OnWheatCollectionStart();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool Debug_DisableScaleAnimation = false;

protected:
	UFUNCTION()
	void OnCollectByVehicle(AActor* OverlappedActor, AActor* OtherActor);

private:
	bool m_shouldGrowFullWheat = false;
	
	float m_delayTimer = 0.f;

	void GrowSprout(float targetScale, float deltaTime = 0.f);
};
