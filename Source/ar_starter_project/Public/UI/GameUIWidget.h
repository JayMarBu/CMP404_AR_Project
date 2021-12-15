// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMODEvent.h"
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
		UTextBlock* EnemiesRemainingText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* WaveCountText;

	UPROPERTY(meta = (BindWidget))
		UButton* MenuButton;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* HealthBar;

	TArray<UUserWidget*> m_hearts;

	UPROPERTY(EditAnywhere)
		UFMODEvent* TestEvent;

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

	virtual void SetEnemiesRemaining(const unsigned int& currentEnemiesRemaining, const unsigned int& totalEneimes);
	virtual void SetWaveCount(const unsigned int& num);

	UFUNCTION()
	void MenuButtonPressed();
};
