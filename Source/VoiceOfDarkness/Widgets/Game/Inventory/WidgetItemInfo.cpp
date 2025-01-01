// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetItemInfo.h"

#include "Components/Image.h"

void UWidgetItemInfo::UpdateItemInfo(const FInventoryInfoStruct ItemData)
{
	IMG_Item->SetBrushFromTexture(ItemData.ItemTexture);
}
