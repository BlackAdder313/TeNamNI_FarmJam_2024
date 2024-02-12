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
	void BeginPlay() override;

	void UpdateVehiclePositions(const TArray<FVector>& vehiclePositions);

protected:
	UFUNCTION()
	void OnPlayerEnterTriggerArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> WheatActor;

private:

	void UpdateWheatPositionsToSprout(const FVector& position, const TArray<FVector>& sproutPositions);

	FVector m_origin;
	FVector m_areaBounds;

	int m_verticalWheatAmount = 0;
	int m_horizontalWheatAmount = 0;

	TArray<FVector> m_wheatPositions_topLeft;
	TArray<FVector> m_wheatPositions_topRight;
	TArray<FVector> m_wheatPositions_bottomLeft;
	TArray<FVector> m_wheatPositions_bottomRight;

	TArray<FVector> m_wheatPositionsToSprout;
};
