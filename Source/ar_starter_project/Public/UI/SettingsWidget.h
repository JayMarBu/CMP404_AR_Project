// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	// Members ************************************************************************************
protected:
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* HighscoreText;

	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
		UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
		UButton* InstructionsButton;

	// Methods ************************************************************************************

protected:
	void NativeConstruct() override;

public:

	UFUNCTION()
		void PlayButtonPress();
};
