// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUDPlayerController.h"
#include "GameplayHUD.h"

AGameplayHUDPlayerController::AGameplayHUDPlayerController()
{

}

void AGameplayHUDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AGameplayHUDPlayerController::OpenMenu);
	}
}

void AGameplayHUDPlayerController::OpenMenu()
{
	if (AGameplayHUD* Hud = Cast<AGameplayHUD>(GetHUD()))
	{
		Hud->ShowMenu();
	}
}
