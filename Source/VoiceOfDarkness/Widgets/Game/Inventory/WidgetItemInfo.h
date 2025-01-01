// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "WidgetItemInfo.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetItemInfo : public UUserWidget
{
	GENERATED_BODY()

public:


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SelfWidget", meta = (BindWidget))
	UImage* IMG_Item;

public:

	UFUNCTION()
	void UpdateItemInfo(const FInventoryInfoStruct ItemData);
};
