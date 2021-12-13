// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHud.generated.h"

class UMainMenuUserWidget;
class UGameUIWidget;

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
	UGameUIWidget* m_gameHUDWidget;

	TSharedPtr<class SGameplayWidget> GameplayWidget;
	TSharedPtr<class SWidget> GameplayWidgetContainer;

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuUserWidget> m_mainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameUIWidget> m_gameUIWidgetClass;

	// Methods ************************************************************************************

protected:
	
	virtual void BeginPlay() override;

public:

	AMainMenuHud();
	
	virtual void ShowMainMenu();
	virtual void HideMainMenu();

	virtual void ShowGameHUD();
	virtual void HideGameHUD();

	void SpawnEnemy();
	void SpawnControllerEnemy();

	virtual void ShowDebugMenu();
	virtual void RemoveDebugMenu();

	virtual void SpawnHearts(const unsigned int& num);
	virtual void SetCurrentHealth(const unsigned int& num);


	virtual void DrawHUD() override;
	

};
