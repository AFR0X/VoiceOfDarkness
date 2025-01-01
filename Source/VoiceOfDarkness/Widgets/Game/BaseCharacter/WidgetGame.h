// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetGame.generated.h"

class UWidgetProgress;
class UWidgetAnimation;
class UVerticalBox;
class UInputAction;
class UWidgetInteractInfo;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetGame : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void UpdateFlashlightProgress(float CurrentAmount, float MaxAmount);
	UFUNCTION()
	void UpdateSprintProgress(float CurrentAmount, float MaxAmount);

	UFUNCTION()
	void UpdateInteractInfo(UInputAction* InputAction, FText InteractText);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetProgress", meta = (BindWidget))
	UWidgetProgress* WB_Progress_Flashlight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetProgress", meta = (BindWidget))
	UWidgetProgress* WB_Progress_Sprint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetProgress", meta = (BindWidget))
	UVerticalBox* Vbox_Info;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetInteractInfo")
	TSubclassOf<UWidgetInteractInfo> WidgetInteractInfoClass = nullptr;
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SprintAnim;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FlashlightAnim;
};
