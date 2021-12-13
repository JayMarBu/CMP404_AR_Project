// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathScreenWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SphereWorldGameState.h"

void UDeathScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddUniqueDynamic(this, &UDeathScreenWidget::PlayButtonPress);
	ReturnButton->OnClicked.AddUniqueDynamic(this, &UDeathScreenWidget::ReturnButtonPress);
}

UDeathScreenWidget::UDeathScreenWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{

}

void UDeathScreenWidget::PlayButtonPress()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SetGameState(ARGameStates::Gameplay);
}

void UDeathScreenWidget::ReturnButtonPress()
{
	// go to home screen
}

void UDeathScreenWidget::SetScore(const unsigned int& num)
{
	FString text = "Score: " + FString::FromInt(num);

	ScoreText->SetText(FText::FromString(text));
}
