// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UNeedforWheatWheelRear::UNeedforWheatWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}