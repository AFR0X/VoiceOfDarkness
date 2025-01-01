// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInventory.generated.h"

class UAC_InventoryComponent;
class UWidgetInventorySlot;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetInventory : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SelfWidget", meta = (BindWidget))
	UUniformGridPanel* UniGrid_Item;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetInventorySlot")
	TSubclassOf<UWidgetInventorySlot> WidgetInventoryClass = nullptr;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UAC_InventoryComponent* InventoryComponentReferance = nullptr;

public:

	void InitInventory(UAC_InventoryComponent* InventoryComponentRef);
	
};
