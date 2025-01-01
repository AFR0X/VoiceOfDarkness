// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceOfDarkness/Widgets/Game/Inventory/WidgetInventorySlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWidgetInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemData.ActorClass)
	{
		if (!Img_Item->IsVisible())
		{
			Img_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		
		Img_Item->SetBrushFromTexture(ItemData.ItemTexture);
		if (ItemData.ItemAmount > 1)
		{
			if (!Txt_Amount->IsVisible())
			{
				Txt_Amount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			
			Txt_Amount->SetText(FText::AsNumber(ItemData.ItemAmount));
		}
	}
}
