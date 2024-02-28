// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWWheat.h"

#include "NeedforWheatGameMode.h"

void ANFWWheat::OnWheatCollectionStart_Implementation()
{
	OnActorHit.AddUniqueDynamic(this, &ANFWWheat::OnCollectByVehicle);

	TArray<UStaticMeshComponent*> wheatParts;
	GetComponents(wheatParts);

	for (auto wheatPart : wheatParts)
	{
		wheatPart->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	}
}

void ANFWWheat::OnCollectByVehicle(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (auto gameMode = Cast<ANeedforWheatGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->OnWheatCollected();
	}

	TArray<UStaticMeshComponent*> wheatParts;
	GetComponents(wheatParts);

	for (auto wheatPart : wheatParts)
	{
		wheatPart->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	Destroy();
}