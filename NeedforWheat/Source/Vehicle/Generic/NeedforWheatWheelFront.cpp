// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UNeedforWheatWheelFront::UNeedforWheatWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}