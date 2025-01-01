// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VoiceOfDarkness/BaseEnum.h"
#include "INT_BaseAnimInstance.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UINT_BaseAnimInstance : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOICEOFDARKNESS_API IINT_BaseAnimInstance
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Animation")
	void SetChangeAnimation(EAnimationCategory ChangeAnimationCategory);
	
};
