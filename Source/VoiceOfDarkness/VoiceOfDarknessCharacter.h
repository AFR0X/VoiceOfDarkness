// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnum.h"
#include "GameFramework/Character.h"
#include "Interfaces/INT_BaseCharacter.h"
#include "InputMappingContext.h"
#include "Logging/LogMacros.h"
#include "VoiceOfDarknessCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UAC_Flashlight;
class UAC_CharacterState;
class AA_MasterInteract;
class UAC_InventoryComponent;
class ABaseGameHud;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AVoiceOfDarknessCharacter : public ACharacter, public IINT_BaseCharacter
{
	GENERATED_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Flashlight Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FlashlightAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	
	/** Flashlight Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UAC_Flashlight* AC_FlashlightComponent;

	/** Flashlight Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UAC_CharacterState* AC_CharacterState;

	/** Inventory Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UAC_InventoryComponent* AC_InventoryComp;

	/** Backpack Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SK_BackPack;
	
public:
	
	AVoiceOfDarknessCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void BeginPlay() override;
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UAC_InventoryComponent* GetInventoryReferance() const { return AC_InventoryComp; }
	FORCEINLINE USkeletalMeshComponent* GetBackPackReferance() const { return SK_BackPack; }

#pragma region BaseGameHud

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseGameHud")
	ABaseGameHud* BaseGameHudReferance = nullptr;

#pragma endregion
	
#pragma region Animations
	
	// Animations
	UFUNCTION()
	void ChangeAnimationSystem(EAnimationCategory SelectAnimation);
	
	UFUNCTION()
	void PlayMontageWitchCallBack(UAnimMontage* MontageToPlay, float PlayRate = 1.f);

	UFUNCTION()
	void OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted);
	
protected:
	
	UPROPERTY()
	EAnimationCategory AnimationCategory = EAnimationCategory::NO_EQUIP;

#pragma endregion

#pragma region Crouch

protected:
	
	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void CrouchRelased();

#pragma endregion

#pragma region Interact

protected:
	
	UFUNCTION()
	void InteractSystem();

#pragma endregion

#pragma region Interfaces

	UFUNCTION()
	virtual AVoiceOfDarknessCharacter* GetBaseCharacterReferance_Implementation() override;

	UFUNCTION()
	virtual bool IsPlayer_Implementation() override;

#pragma endregion
	
};

