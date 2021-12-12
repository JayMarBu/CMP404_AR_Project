// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MainMenuHud.h"
#include "GameplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API AGameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameplayGameMode();

	void BeginGame();
};
