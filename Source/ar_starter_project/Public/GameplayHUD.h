// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayHUD.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()

protected:

	TSharedPtr<class SGameplayWidget> GameplayWidget;
	TSharedPtr<class SWidget> GameplayWidgetContainer;

	virtual void BeginPlay() override;

public: 

	void SpawnEnemy();
	void SpawnControllerEnemy();

	void ShowMenu();
	void RemoveMenu();
	
};
