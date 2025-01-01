// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceOfDarkness/Animations/BaseAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "VoiceOfDarkness/VoiceOfDarknessCharacter.h"
#include "VoiceOfDarkness/Interfaces/INT_BaseCharacter.h"

void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (TryGetPawnOwner())
	{
		BaseCharacterReferance = IINT_BaseCharacter::Execute_GetBaseCharacterReferance(TryGetPawnOwner());
		if (BaseCharacterReferance)
		{
			MovementComponentReferance = BaseCharacterReferance->GetCharacterMovement();
		}
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (TryGetPawnOwner())
	{
		if (BaseCharacterReferance)
		{
			FVector Velocity = TryGetPawnOwner()->GetVelocity();
			FRotator CharacterRotate = TryGetPawnOwner()->GetActorRotation();
			
			Speed = Velocity.Size();
			Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, CharacterRotate);
			bIsFalling = MovementComponentReferance->IsFalling();
			bIsCrouch = MovementComponentReferance->IsCrouching();

			PitchValue = UKismetMathLibrary::NormalizedDeltaRotator(TryGetPawnOwner()->GetControlRotation(), CharacterRotate).Pitch;
			YawValue = UKismetMathLibrary::NormalizedDeltaRotator(TryGetPawnOwner()->GetControlRotation(), CharacterRotate).Yaw;
		}
	}
}

void UBaseAnimInstance::SetChangeAnimation_Implementation(EAnimationCategory ChangeAnimationCategory)
{
	IINT_BaseAnimInstance::SetChangeAnimation_Implementation(ChangeAnimationCategory);

	AnimationCategory = ChangeAnimationCategory;
}
