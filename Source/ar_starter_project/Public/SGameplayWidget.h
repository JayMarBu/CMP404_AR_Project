// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Runtime/SlateCore/Public/Widgets/SOverlay.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SGameplayWidget : public SCompoundWidget
{
	// Members ************************************************************************************
public:

	TWeakObjectPtr<class AMainMenuHud> OwningHUD;

	// Methods ************************************************************************************ 
public:
	SLATE_BEGIN_ARGS(SGameplayWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMainMenuHud>, OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs);

	bool SupportsKeyboardFocus() const override { return true; }

	FReply OnSpawnEnemyDefault() const;

	FReply OnSpawnBaseControllerEnemy() const;
};
