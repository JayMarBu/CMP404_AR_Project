// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayHUDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API AGameplayHUDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	AGameplayHUDPlayerController();

	virtual void SetupInputComponent() override;

	void OpenMenu();
};
