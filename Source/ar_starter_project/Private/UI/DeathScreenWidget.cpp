// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathScreenWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SphereWorldGameState.h"

#include "FMODBlueprintStatics.h"

void UDeathScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialise button callbacks
	PlayButton->OnClicked.AddUniqueDynamic(this, &UDeathScreenWidget::PlayButtonPress);
	ReturnButton->OnClicked.AddUniqueDynamic(this, &UDeathScreenWidget::ReturnButtonPress);

	// Initialise Sound
	TestEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/UI_Sounds/switch_006"));
}

UDeathScreenWidget::UDeathScreenWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{

}

void UDeathScreenWidget::PlayButtonPress()
{
	GetWorld()->GetGameState<ASphereWorldGameState>()->SetGameState(ARGameStates::Gameplay);
	FTransform tr = ASphereWorldGameState::Get(this)->GetPawn()->GetActorTransform();
	UFMODBlueprintStatics::PlayEventAtLocation(this, TestEvent, tr, true);
}

void UDeathScreenWidget::ReturnButtonPress()
{
	ASphereWorldGameState::Get(this)->SetGameState(ARGameStates::Main_Menu);
	FTransform tr = ASphereWorldGameState::Get(this)->GetPawn()->GetActorTransform();
	UFMODBlueprintStatics::PlayEventAtLocation(this, TestEvent, tr, true);
}

void UDeathScreenWidget::SetScore(const unsigned int& num)
{
	FString text = "Score: " + FString::FromInt(num);

	ScoreText->SetText(FText::FromString(text));
}
