// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHud.h"
#include "UI/MainMenuUserWidget.h"
#include "SGameplayWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "SphereWorldGameState.h"

void AMainMenuHud::BeginPlay()
{
	Super::BeginPlay();

	ShowMainMenu();
}

AMainMenuHud::AMainMenuHud()
{
	float test = 0;
}

void AMainMenuHud::ShowMainMenu()
{
	if (m_widgetClass)
	{
		m_mainMenuWidget = CreateWidget<UMainMenuUserWidget>(GetWorld(), m_widgetClass);
		if (m_mainMenuWidget)
		{
			m_mainMenuWidget->AddToViewport();
		}
	}
}

void AMainMenuHud::HideMainMenu()
{
	if (m_mainMenuWidget)
	{
		m_mainMenuWidget->RemoveFromParent();
	}
}

void AMainMenuHud::SpawnEnemy()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SpawnEnemy();
}

void AMainMenuHud::SpawnControllerEnemy()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SpawnControllerEnemy();
}

void AMainMenuHud::ShowDebugMenu()
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

void AMainMenuHud::RemoveDebugMenu()
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

void AMainMenuHud::DrawHUD()
{
	Super::DrawHUD();
}
