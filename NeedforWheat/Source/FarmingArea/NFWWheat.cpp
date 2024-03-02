// Copyright Epic Games, Inc. All Rights Reserved.


#include "NFWWheat.h"

#include "Kismet/KismetMathLibrary.h"

#include "NeedforWheatGameMode.h"
#include "Player\NeedforWheatPawn.h"

void ANFWWheat::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
}

void ANFWWheat::OnWheatCollectionStart_Implementation()
{
	SetActorEnableCollision(true);
	OnActorBeginOverlap.AddDynamic(this, &ANFWWheat::OnCollectByVehicle);
}

void ANFWWheat::OnCollectByVehicle(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const ANeedforWheatPawn* pawnActor = Cast<ANeedforWheatPawn>(OtherActor))
	{
		if (auto gameMode = Cast<ANeedforWheatGameMode>(GetWorld()->GetAuthGameMode()))
		{
			gameMode->OnWheatCollected();
		}

		SetActorEnableCollision(false);

		SetLifeSpan(UKismetMathLibrary::RandomFloatInRange(0.2, 0.6));
	}
}