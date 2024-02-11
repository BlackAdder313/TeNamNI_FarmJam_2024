// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWFarmingAreaTrigger.h"

void ANFWFarmingAreaTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	OnActorBeginOverlap.AddDynamic(this, &ANFWFarmingAreaTrigger::OnPlayerEnterTriggerArea);
	OnActorEndOverlap.AddDynamic(this, &ANFWFarmingAreaTrigger::OnPlayerExitTriggerArea);
}

void ANFWFarmingAreaTrigger::OnPlayerEnterTriggerArea(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered"));
}

void ANFWFarmingAreaTrigger::OnPlayerExitTriggerArea(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Left"));
}