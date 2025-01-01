// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VoiceOfDarkness/BaseEnum.h"
#include "VoiceOfDarkness/Interfaces/INT_BaseAnimInstance.h"
#include "BaseAnimInstance.generated.h"

class AVoiceOfDarknessCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API UBaseAnimInstance : public UAnimInstance, public  IINT_BaseAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void SetChangeAnimation_Implementation(EAnimationCategory ChangeAnimationCategory) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	AVoiceOfDarknessCharacter* BaseCharacterReferance = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	UCharacterMovementComponent* MovementComponentReferance = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Direction = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsFalling = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	EAnimationCategory AnimationCategory = EAnimationCategory::NO_EQUIP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float PitchValue = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float YawValue = 0.0f;
};
