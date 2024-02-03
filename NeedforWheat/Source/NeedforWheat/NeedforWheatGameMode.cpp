// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatGameMode.h"
#include "NeedforWheatPlayerController.h"

ANeedforWheatGameMode::ANeedforWheatGameMode()
{
	PlayerControllerClass = ANeedforWheatPlayerController::StaticClass();
}
