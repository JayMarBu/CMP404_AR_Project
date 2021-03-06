// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMODEvent.h"
#include "MainMenuUserWidget.generated.h"


class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	// Members ************************************************************************************
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(EditAnywhere)
	UFMODEvent* TestEvent;

	// Methods ************************************************************************************
protected:
	void NativeConstruct() override;

public:
	
	UMainMenuUserWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		void PlayButtonPress();

	UFUNCTION()
		void SettingsButtonPress();
};
