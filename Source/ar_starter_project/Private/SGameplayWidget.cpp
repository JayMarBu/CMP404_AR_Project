// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayWidget.h"
#include "GameplayHUD.h"
//#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#define BASIC_BUTTON(label, fontStyle, justification, callback)\
SNew(SButton).OnClicked(this, &callback)\
[\
	SNew(STextBlock)\
	.Font(fontStyle)\
	.Text(label)\
	.Justification(justification)\
]

#define LOCTEXT_NAMESPACE "GameplayMenu"

void SGameplayWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	OwningHUD = inArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(50.0f, 50.0f);
	const FMargin ButtonPadding = FMargin(10.0f);

	const FText TitleText = LOCTEXT("Debug Menu", "Debug Menu");
	const FText PlayText = LOCTEXT("Spawn Enemy Button", "Spawn Enemy");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 40.0f;

	ChildSlot
	[
		// black background
		SNew(SOverlay) + SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor(0,0,0,0))
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(ContentPadding)
		[
			// Title text
			SNew(SVerticalBox) + SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Font(TitleTextStyle)
				.Text(TitleText)
				.Justification(ETextJustify::Center)
			]

			// Spawn Enemy Button
			+ SVerticalBox::Slot().Padding(ButtonPadding)[BASIC_BUTTON(PlayText, ButtonTextStyle, ETextJustify::Center, SGameplayWidget::OnPlayClicked)]
		]
	];
}

FReply SGameplayWidget::OnPlayClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->SpawnEnemy();
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE