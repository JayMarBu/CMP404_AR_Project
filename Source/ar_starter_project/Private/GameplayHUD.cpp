// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUD.h"
#include "SGameplayWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "SphereWorldGameState.h"

void AGameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMenu();
}

void AGameplayHUD::SpawnEnemy()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SpawnEnemy();
}

void AGameplayHUD::SpawnControllerEnemy()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SpawnControllerEnemy();
}

void AGameplayHUD::ShowMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		GameplayWidget = SNew(SGameplayWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameplayWidgetContainer, SWeakWidget).PossiblyNullContent(GameplayWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			//PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AGameplayHUD::RemoveMenu()
{
	if (GEngine && GEngine->GameViewport && GameplayWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(GameplayWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			//PlayerOwner->SetInputMode(FInputModeGameAndUI());
		}
	}
}
