// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetInventory.h"
#include "WidgetInventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "VoiceOfDarkness/Components/AC_InventoryComponent.h"


void UWidgetInventory::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->SetShowMouseCursor(true);
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	GetOwningPlayer()->SetIgnoreMoveInput(true);
	SetUserFocus(GetOwningPlayer());
}

FReply UWidgetInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	 Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FEventReply Reply;

	 if (InKeyEvent.GetKey() == EKeys::Tab)
	 {
	 	GetOwningPlayer()->SetShowMouseCursor(false);
	 	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	 	GetOwningPlayer()->ResetIgnoreMoveInput();
	 	
		InventoryComponentReferance->PressedInventory();

	 	return Reply.NativeReply;
	 }


	return Reply.NativeReply;
}


void UWidgetInventory::InitInventory(UAC_InventoryComponent* InventoryComponentRef)
{
	InventoryComponentReferance = InventoryComponentRef;
	if (InventoryComponentReferance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR NO VALID InventoryReferance (WidgetInventory)"));
		return;
	}
	
	if (WidgetInventoryClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR NO VALID WidgetInventorySlotClass (WidgetInventory)"));
		return;
	}

	UniGrid_Item->ClearChildren();
	int32 InventoryIndexSize = InventoryComponentReferance->InventoryData.Num();
	int32 SlotSize = InventoryComponentReferance->CurrentSlotSize;

	for (int32 i = 0; i < InventoryIndexSize; i++)
	{
		if (i >= SlotSize)
		{
			break;
		}
		else
		{
			UWidgetInventorySlot* WidgetInventorySlot = CreateWidget<UWidgetInventorySlot>(GetWorld(), WidgetInventoryClass);
			if (WidgetInventorySlot)
			{
				WidgetInventorySlot->ItemData = InventoryComponentReferance->InventoryData[i];
				UniGrid_Item->AddChildToUniformGrid(WidgetInventorySlot, i / 4, i % 4);
			}
		}
	}
}
