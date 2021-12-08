// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameMode.h"
#include "GameplayHUD.h"
#include "CustomARPawn.h"
#include "GameplayHUDPlayerController.h"
#include "SphereWorldGameState.h"

AGameplayGameMode::AGameplayGameMode()
{
	DefaultPawnClass = ACustomARPawn::StaticClass();

	PlayerControllerClass = AGameplayHUDPlayerController::StaticClass();
	HUDClass = AGameplayHUD::StaticClass();

	GameStateClass = ASphereWorldGameState::StaticClass();

}
