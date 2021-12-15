// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHud.generated.h"

class UMainMenuUserWidget;
class UGameUIWidget;
class UDeathScreenWidget;
class USettingsWidget;

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API AMainMenuHud : public AHUD
{
	GENERATED_BODY()
	// Members ************************************************************************************
protected:
	UMainMenuUserWidget* m_mainMenuWidget;
	UGameUIWidget* m_gameHUDWidget;
	UDeathScreenWidget* m_deathWidget;
	USettingsWidget* m_settingsWidget;

	// Debug Widgets
	TSharedPtr<class SGameplayWidget> GameplayWidget;
	TSharedPtr<class SWidget> GameplayWidgetContainer;

public:
	// Blueprint widget type containers
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuUserWidget> m_mainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameUIWidget> m_gameUIWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDeathScreenWidget> m_deathWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USettingsWidget> m_settingsWidgetClass;

	// Methods ************************************************************************************
protected:
	
	virtual void BeginPlay() override;

public:

	AMainMenuHud();

	// Show/Hide Menus
	virtual void ShowMainMenu();
	virtual void HideMainMenu();

	virtual void ShowGameHUD();
	virtual void HideGameHUD();
	
	virtual void ShowDeathScreen();
	virtual void HideDeathScreen();

	virtual void ShowSettingsScreen();
	virtual void HideSettingsScreen();

	// Debug menu functions
	virtual void ShowDebugMenu();
	virtual void HideDebugMenu();

	void SpawnEnemy();
	void SpawnControllerEnemy();

	// Update UI methods
	virtual void SpawnHearts(const unsigned int& num);
	virtual void SetCurrentHealth(const unsigned int& num);
	virtual void SetEnemyCount(const unsigned int& currentEnemiesRemaining, const unsigned int& totalEneimes);
	virtual void SetWaveNum(const unsigned int& num);
	virtual void SetScore(const unsigned int& num);


	virtual void DrawHUD() override;
	

};
