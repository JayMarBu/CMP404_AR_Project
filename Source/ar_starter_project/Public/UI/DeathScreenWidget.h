// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathScreenWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UDeathScreenWidget : public UUserWidget
{
	GENERATED_BODY()

	// Members ************************************************************************************
protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
		UButton* ReturnButton;
	

	// Methods ************************************************************************************
protected:
	void NativeConstruct() override;

public:

	UDeathScreenWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		void PlayButtonPress();

	UFUNCTION()
		void ReturnButtonPress();

	void SetScore(const unsigned int& num);
};
