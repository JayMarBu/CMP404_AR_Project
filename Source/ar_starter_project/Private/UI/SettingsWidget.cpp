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
	OptionsButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::SettingsButtonPressed);

	// Other callbacks set in blueprint

	TestEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/UI_Sounds/switch_006"));
}

void USettingsWidget::PlayButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Gameplay);
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void USettingsWidget::SettingsButtonPressed()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Main_Menu);
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void USettingsWidget::OnSliderChange(float Value)
{
	ASphereWorldGameState::Get(this)->SetVolume(Value);
}

void USettingsWidget::OnToggle(bool bIsChecked)
{
	if(!bIsChecked)
		UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
	ASphereWorldGameState::Get(this)->SetMuteState(bIsChecked);
	if(bIsChecked)
		UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

float USettingsWidget::GetVolume()
{
	return ASphereWorldGameState::Get(this)->GetVolume();
}

bool USettingsWidget::GetMuteState()
{
	return ASphereWorldGameState::Get(this)->GetMuteState();
}
