// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUIWidget.generated.h"

class UTextBlock;
class UButton;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UGameUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// Members ************************************************************************************
protected:
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		UButton* MenuButton;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* HealthBar;

	TArray<UUserWidget*> m_hearts;

public:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> m_heartWidgetClass;


	// Methods ************************************************************************************
protected:
	 virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SpawnHearts(const unsigned int& num);

	UFUNCTION()
	void SetHealth(const unsigned int& num);

	virtual void SetScore(const unsigned int& num);

	UFUNCTION()
	void MenuButtonPressed();

	//UGameUIWidget(const FObjectInitializer& ObjectInitializer);
};
