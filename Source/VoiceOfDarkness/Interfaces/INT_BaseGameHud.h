// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_BaseGameHud.generated.h"

class ABaseGameHud;
class UInputAction;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UINT_BaseGameHud : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOICEOFDARKNESS_API IINT_BaseGameHud
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseGameHud")
	ABaseGameHud* GetBaseGameHudReferance();
	
};
