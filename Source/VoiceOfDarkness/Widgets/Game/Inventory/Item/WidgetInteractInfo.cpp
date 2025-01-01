// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetInteractInfo.h"

#include "EnhancedInputSubsystems.h"
#include "Components/TextBlock.h"

void UWidgetInteractInfo::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UWidgetInteractInfo::UpdateInteractInfo(UInputAction* Input, FText InteractInfoText)
{
	if (Input)
	{
		// Add Input Mapping Context
		if (APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				FText InputText = Subsystem->QueryKeysMappedToAction(Input)[0].GetDisplayName(true);

				Txt_Input->SetText(InputText);
				Txt_Interact->SetText(InteractInfoText);
			}
		}
	}
}
