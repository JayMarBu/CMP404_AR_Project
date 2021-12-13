// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUIWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UGameUIWidget::NativeConstruct()
{

}

void UGameUIWidget::SpawnHearts(const unsigned int& num)
{
	//m_hearts.clear();

	if(!m_heartWidgetClass)
		return;


	FSlateChildSize slateSize{};
	for (unsigned int i = 0; i < num; i++)
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), m_heartWidgetClass);
		if (widget)
		{
			//widget->Slot;
			m_hearts.Add(widget);
			//widget->AddToViewport();
			HealthBar->AddChildToHorizontalBox(widget)->SetSize(slateSize);
		}
		
	}
}

void UGameUIWidget::SetHealth(const unsigned int& num)
{
	for (unsigned int i = 0; i < (unsigned int)m_hearts.Num(); i++)
	{
		if( i < num)
		{
			m_hearts[i]->SetColorAndOpacity(FLinearColor(0,1,0,1));
		}
		else
		{
			m_hearts[i]->SetColorAndOpacity(FLinearColor(0.25,0.25,0.25,1));
		}
	}
}
