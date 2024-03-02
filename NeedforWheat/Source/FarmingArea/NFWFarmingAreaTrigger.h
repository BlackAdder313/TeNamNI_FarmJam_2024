// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"

#include "NFWFarmingAreaTrigger.generated.h"

struct FWheatSpawner
{
	FWheatSpawner(float delayTimer, const TPair<int, int>& positionIndiced) : m_delayTimer(delayTimer),
																			  m_spawnPositionIndices(positionIndiced) {}

	bool Tick(float deltaTime)
	{
		m_delayTimer -= deltaTime;
		return m_delayTimer < 0.f;
	}

	bool operator == (const FWheatSpawner& Other) const
	{
		return m_spawnPositionIndices == Other.m_spawnPositionIndices;
	}

	float m_delayTimer = 0.f;
	TPair<int, int> m_spawnPositionIndices;	
};

/**
 *  Rear wheel definition for Sports Car.
 */
UCLASS()
class NEEDFORWHEAT_API ANFWFarmingAreaTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ANFWFarmingAreaTrigger() = default;
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	void RegisterWheatToSpawn(const TPair<int, int>& spawnPositionIndices);
	void DelayedSpawnWheat(float DeltaTime);
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

	TArray<TPair<int, int>> m_wheatPositionsToSprout;
	TArray <TWeakObjectPtr<AActor>> m_actorsToIgnore;

	TArray<FWheatSpawner> m_wheatSpawners;
	TArray<FWheatSpawner> m_wheatSpawnersToClear;
};
