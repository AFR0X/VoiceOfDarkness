// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInteractInfo.generated.h"

class UInputAction;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetInteractInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateInteractInfo(UInputAction* Input, FText InteractInfoText);
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SelfWidget", meta =(BindWidget))
	UTextBlock* Txt_Input;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SelfWidget", meta =(BindWidget))
	UTextBlock* Txt_Interact;
	
};
