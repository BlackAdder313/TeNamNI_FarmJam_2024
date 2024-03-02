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

	TPair<int, int> GetPlantedWheatInfo() const;

protected:
	UFUNCTION()
	void OnPlayerEnterTriggerArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> WheatActor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float WheatSpacing = 65.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool Debug_SpawnWheatOnBeginPlay = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool Debug_SpawnWheatOnExitFromFarmingArea = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int WheatSproutRadius = 3;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int MaxNumberOfWheat = -1;

private:
	bool TryInsertWheatPositionsAroundIndex(uint16_t index, float heightOffset, float widthOffset);

	void UpdateWheatPositionsToSprout(const FVector& position, const TArray<FVector>& sproutPositions, const uint16_t verticalIndex);
	void TrySpreadWheatToSprout(uint16_t verticalIndex, uint16_t horizontalIndex, uint16_t horizontalOffset);

	void SpawnWheat(FVector position);

	FVector m_origin;
	FVector m_areaBounds;

	int m_verticalWheatHalfAmount = 0;
	int m_horizontalWheatHalfAmount = 0;

	TArray<FVector> m_wheatPositions_topLeft;
	TArray<FVector> m_wheatPositions_topRight;
	TArray<FVector> m_wheatPositions_bottomLeft;
	TArray<FVector> m_wheatPositions_bottomRight;

	TArray<TArray<FVector>> m_wheatPositions;

	TArray<FVector> m_wheatPositionsToSprout;
	TWeakObjectPtr<AActor> m_actorToIgnore;
};
