// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SphereWorldGameState.h"


UMainMenuUserWidget::UMainMenuUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	
}

void UMainMenuUserWidget::PlayButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Gameplay);
}

void UMainMenuUserWidget::InfoButtonPress()
{

}

void UMainMenuUserWidget::SettingsButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Options_Menu);
}

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::PlayButtonPress);
	OptionsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::SettingsButtonPress);
	InstructionsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::InfoButtonPress);
}

