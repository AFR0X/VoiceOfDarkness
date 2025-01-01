// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetGame.h"

#include "Components/VerticalBox.h"
#include "Progress/WidgetProgress.h"
#include "VoiceOfDarkness/Widgets/Game/Inventory/Item/WidgetInteractInfo.h"


void UWidgetGame::UpdateFlashlightProgress(float CurrentAmount, float MaxAmount)
{
	if (WB_Progress_Flashlight)
	{
		WB_Progress_Flashlight->UpdateProgressBar(CurrentAmount, MaxAmount);
	}
}

void UWidgetGame::UpdateSprintProgress(float CurrentAmount, float MaxAmount)
{
	if (WB_Progress_Sprint)
	{
		WB_Progress_Sprint->UpdateProgressBar(CurrentAmount, MaxAmount);
	}
}

void UWidgetGame::UpdateInteractInfo(UInputAction* InputAction, FText InteractText)
{
	if (!Vbox_Info->GetAllChildren().IsEmpty())
	{
		Vbox_Info->ClearChildren();
	}
	
	if (InputAction)
	{
		UWidgetInteractInfo* WidgetInteractInfo = CreateWidget<UWidgetInteractInfo>(GetWorld(), WidgetInteractInfoClass);
		if (WidgetInteractInfo)
		{
			WidgetInteractInfo->UpdateInteractInfo(InputAction, InteractText);
			Vbox_Info->AddChildToVerticalBox(WidgetInteractInfo);
		}
	}
}
