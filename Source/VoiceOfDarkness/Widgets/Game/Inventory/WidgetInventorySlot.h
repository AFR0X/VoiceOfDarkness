// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "WidgetInventorySlot.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SelfWidget", meta = (BindWidget))
	UImage* Img_Item;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SelfWidget", meta = (BindWidget))
	UTextBlock* Txt_Amount;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	FInventoryInfoStruct ItemData;
};
