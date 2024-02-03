// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeedforWheatPawn.h"
#include "NeedforWheatSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class NEEDFORWHEAT_API ANeedforWheatSportsCar : public ANeedforWheatPawn
{
	GENERATED_BODY()
	
public:

	ANeedforWheatSportsCar();
};
