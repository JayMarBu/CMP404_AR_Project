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
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("Play Pressed")));
	UWorld* w = GetWorld();

	ASphereWorldGameState* gs = w->GetGameState<ASphereWorldGameState>();
	APlayerController* pc = w->GetFirstPlayerController();

	gs->SetPawn(pc->GetPawn<ACustomARPawn>());

	gs->BeginGame();
}

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::PlayButtonPress);
}

