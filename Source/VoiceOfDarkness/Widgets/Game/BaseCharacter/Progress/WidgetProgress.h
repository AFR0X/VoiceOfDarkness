// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetProgress.generated.h"

class UProgressBar;
class UImage;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UWidgetProgress : public UUserWidget
{
	GENERATED_BODY()

public:


	UFUNCTION()
	void UpdateProgressBar(float CurrentValue, float MaxValue);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfWidget")
	UProgressBar* ProgressBar;
	
	
};
