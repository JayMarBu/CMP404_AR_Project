// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SphereWorldGameState.h"

#include "FMODBlueprintStatics.h"

UMainMenuUserWidget::UMainMenuUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	
}

void UMainMenuUserWidget::PlayButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Gameplay);
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void UMainMenuUserWidget::InfoButtonPress()
{
	FTransform tr = ASphereWorldGameState::Get(this)->GetPawn()->GetActorTransform();
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void UMainMenuUserWidget::SettingsButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Options_Menu);
	UFMODBlueprintStatics::PlayEvent2D(this, TestEvent, true);
}

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::PlayButtonPress);
	OptionsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::SettingsButtonPress);
	InstructionsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::InfoButtonPress);

	TestEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/UI_Sounds/switch_006"));
}

