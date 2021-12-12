// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHud.generated.h"

class UMainMenuUserWidget;

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API AMainMenuHud : public AHUD
{
	GENERATED_BODY()
	// Members ************************************************************************************
protected:
	UPROPERTY();
	UMainMenuUserWidget* m_mainMenuWidget;

	TSharedPtr<class SGameplayWidget> GameplayWidget;
	TSharedPtr<class SWidget> GameplayWidgetContainer;

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuUserWidget> m_widgetClass;

	// Methods ************************************************************************************

protected:
	
	virtual void BeginPlay() override;

public:

	AMainMenuHud();
	
	virtual void ShowMainMenu();

	virtual void HideMainMenu();


	void SpawnEnemy();
	void SpawnControllerEnemy();

	void ShowDebugMenu();
	void RemoveDebugMenu();


	virtual void DrawHUD() override;
	

};
