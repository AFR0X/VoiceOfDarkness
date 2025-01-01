// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/INT_BaseGameHud.h"
#include "BaseGameHud.generated.h"

class UWidgetGame;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API ABaseGameHud : public AHUD, public  IINT_BaseGameHud
{
	GENERATED_BODY()

public:

	ABaseGameHud();
	
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameWidget")
	UWidgetGame* WidgetGameReferance = nullptr;

	// Functions
	UFUNCTION()
	void UpdateFlashlightWidget(float CurrentValue, float MaxValue);
	UFUNCTION()
	void UpdateSprintWidget(float CurrentValue, float MaxAmount);

	UFUNCTION()
	void ViewInteractInfo(UInputAction* InteractInput, FText InteractText);
protected:

	UPROPERTY()
	TSubclassOf<UWidgetGame> WidgetGameClass = nullptr;


public:

	// Interfaces

	UFUNCTION()
	virtual ABaseGameHud* GetBaseGameHudReferance_Implementation() override;
	
};
