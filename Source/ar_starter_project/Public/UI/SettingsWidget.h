// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMODEvent.h"
#include "SettingsWidget.generated.h"

class UTextBlock;
class UButton;
class USlider;
class UCheckBox;
/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	// Members ************************************************************************************
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* InstructionsButton;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MuteVFXToggle;

	UPROPERTY(meta = (BindWidget))
	USlider* VFXSlider;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MuteMusicToggle;

	UPROPERTY(meta = (BindWidget))
	USlider* MusicSlider;

	UPROPERTY(EditAnywhere)
	UFMODEvent* TestEvent;

	// Methods ************************************************************************************

protected:
	void NativeConstruct() override;

public:

	UFUNCTION()
	void PlayButtonPress();

	UFUNCTION()
	void InfoButtonPressed();

	UFUNCTION()
	void SettingsButtonPressed();
};
