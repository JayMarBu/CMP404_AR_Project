// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SettingsWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "SphereWorldGameState.h"

#include "FMODBlueprintStatics.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::PlayButtonPress);
	InstructionsButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::InfoButtonPressed);

	TestEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/UI_Sounds/switch_006"));
}

void USettingsWidget::PlayButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Main_Menu);
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void USettingsWidget::InfoButtonPressed()
{
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void USettingsWidget::SettingsButtonPressed()
{
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}
