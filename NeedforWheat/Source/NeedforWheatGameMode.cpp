// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeedforWheatGameMode.h"
#include "Player\NeedforWheatPlayerController.h"

ANeedforWheatGameMode::ANeedforWheatGameMode()
{
	PlayerControllerClass = ANeedforWheatPlayerController::StaticClass();
}
