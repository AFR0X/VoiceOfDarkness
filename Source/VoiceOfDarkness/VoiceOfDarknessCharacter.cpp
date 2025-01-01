// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoiceOfDarknessCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/AC_CharacterState.h"
#include "Components/AC_Flashlight.h"
#include "Components/AC_InventoryComponent.h"
#include "GameFramework/HUD.h"
#include "Interfaces/INT_BaseAnimInstance.h"
#include "Interfaces/INT_BaseGameHud.h"
#include "Interfaces/INT_Interact.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AVoiceOfDarknessCharacter

AVoiceOfDarknessCharacter::AVoiceOfDarknessCharacter()
{
	// Base Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-103), FRotator(0,-90,0));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> BaseMeshFinder (TEXT("/Game/YI_NPC/Meshes/Mike/Skeletal/SK_Mike_Body.SK_Mike_Body"));
	if (BaseMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BaseMeshFinder.Object);

		ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceFinder (TEXT("/Game/VoiceOfDarkness/BaseCharacter/Animations/ABP_Anim"));
		if (AnimInstanceFinder.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceFinder.Class);
		}
	}
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), FName("S_HeadCamera")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Inputs
	ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingFinder (TEXT("/Script/EnhancedInput.InputMappingContext'/Game/VoiceOfDarkness/Input/IMC_Default.IMC_Default'"));
	if (InputMappingFinder.Succeeded())
	{
		DefaultMappingContext = InputMappingFinder.Object;

		ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Jump.IA_Jump'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Move.IA_Move'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Look.IA_Look'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Sprint.IA_Sprint'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionInventoryFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Inventory.IA_Inventory'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionFlashlightFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Flashlight.IA_Flashlight'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Crouch.IA_Crouch'"));
		ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractFinder (TEXT("/Script/EnhancedInput.InputAction'/Game/VoiceOfDarkness/Input/Actions/IA_Interact.IA_Interact'"));
		if (InputActionJumpFinder.Succeeded() && InputActionMoveFinder.Succeeded() && InputActionLookFinder.Succeeded() && InputActionSprintFinder.Succeeded() && InputActionInventoryFinder.Succeeded()
			&& InputActionFlashlightFinder.Succeeded() && InputActionCrouchFinder.Succeeded())
		{
			JumpAction = InputActionJumpFinder.Object;
			MoveAction = InputActionMoveFinder.Object;
			LookAction = InputActionLookFinder.Object;
			SprintAction = InputActionSprintFinder.Object;
			InventoryAction = InputActionInventoryFinder.Object;
			FlashlightAction = InputActionFlashlightFinder.Object;
			CrouchAction = InputActionCrouchFinder.Object;
			InteractAction = InputActionInteractFinder.Object;
		}
	}

	// Inventory
	AC_InventoryComp = CreateDefaultSubobject<UAC_InventoryComponent>(TEXT("AC_InventoryComp"));
	// Flashlight
	AC_FlashlightComponent = CreateDefaultSubobject<UAC_Flashlight>(TEXT("AC_FlashlightComponent"));
	// Character State
	AC_CharacterState = CreateDefaultSubobject<UAC_CharacterState>(TEXT("AC_CharacterState"));

	// Backpack Component Creation
	SK_BackPack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));

	// Check to ensure the mesh exists and attach it to the character's skeleton at the correct socket
	if (SK_BackPack)
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> BackpackFinder (TEXT("/Script/Engine.SkeletalMesh'/Game/VoiceOfDarkness/Meshes/Backpack/SK_BackPack.SK_BackPack'"));
		if (BackpackFinder.Succeeded())
		{
			SK_BackPack->SetSkeletalMesh(BackpackFinder.Object);
			SK_BackPack->SetupAttachment(GetMesh(), TEXT("spine_02")); // Attach to the spine_02 socket
			SK_BackPack->SetCollisionResponseToAllChannels(ECR_Ignore);
			SK_BackPack->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable unnecessary collisions
			// Enable shadow casting for FPS view
			SK_BackPack->SetCastShadow(true); // Ensure the backpack casts shadows in the game world

			// Adjust visibility settings
			SK_BackPack->SetOnlyOwnerSee(false); // Backpack should be visible to everyone in the world

			// Optional: Fine-tune rendering for FPS camera
			SK_BackPack->bOwnerNoSee = false; // Backpack is visible in the owner's first-person view
			SK_BackPack->bCastDynamicShadow = true; // Enable dynamic shadows for better visual quality
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AVoiceOfDarknessCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AVoiceOfDarknessCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVoiceOfDarknessCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVoiceOfDarknessCharacter::Look);

		// Flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, AC_FlashlightComponent, &UAC_Flashlight::FlashlightToggleSystem);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, AC_CharacterState, &UAC_CharacterState::StartRunning);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, AC_CharacterState, &UAC_CharacterState::StopRunning);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AVoiceOfDarknessCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AVoiceOfDarknessCharacter::CrouchRelased);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, AC_InventoryComp, &UAC_InventoryComponent::PressedInventory);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AVoiceOfDarknessCharacter::InteractSystem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AVoiceOfDarknessCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AVoiceOfDarknessCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AVoiceOfDarknessCharacter::BeginPlay()
{
	Super::BeginPlay();

	BaseGameHudReferance = IINT_BaseGameHud::Execute_GetBaseGameHudReferance(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD());
	
	if (AC_FlashlightComponent)
	{
		AC_FlashlightComponent->InitFlashlightComponent(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Valid AC_FlashlightComponentRef (BaseCharacter)"));
	}
	
	if (AC_CharacterState)
	{
		AC_CharacterState->InitCharacterState(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Valid AC_FlashlightComponentRef (BaseCharacter)"));
	}

	if (AC_InventoryComp)
	{
		AC_InventoryComp->InitInventoryComp(this);
	}
}

#pragma	region Animations

// Animations
void AVoiceOfDarknessCharacter::ChangeAnimationSystem(EAnimationCategory SelectAnimation)
{
	AnimationCategory = SelectAnimation;
	if (GetMesh()->GetAnimInstance())
	{
		IINT_BaseAnimInstance::Execute_SetChangeAnimation(GetMesh()->GetAnimInstance(), AnimationCategory);
	}
}

void AVoiceOfDarknessCharacter::PlayMontageWitchCallBack(UAnimMontage* MontageToPlay, float PlayRate)
{
	if (!MontageToPlay) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay, PlayRate);

		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &AVoiceOfDarknessCharacter::OnMontageCompleted);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
	}
}

void AVoiceOfDarknessCharacter::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{	
	if (bInterrupted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Montaj kesildi!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Montaj tamamlandı!"));
	}
}

#pragma endregion

void AVoiceOfDarknessCharacter::CrouchPressed()
{
	Crouch();
}

void AVoiceOfDarknessCharacter::CrouchRelased()
{
	UnCrouch();
}

#pragma region Interfaces

void AVoiceOfDarknessCharacter::InteractSystem()
{
	TArray<AActor*> OverlapActors; // Çarpışma alanındaki aktörleri depolamak için bir liste oluşturun
	GetOverlappingActors(OverlapActors, AActor::StaticClass()); // Aktörleri doldur
	if (OverlapActors.Num() > 0)
	{
		AActor* InteractActor = OverlapActors[0];
		if (InteractActor->GetClass()->ImplementsInterface(UINT_Interact::StaticClass()))
		{
			IINT_Interact::Execute_Interactable(InteractActor, this);
		}
	}
}

// Character Referance
AVoiceOfDarknessCharacter* AVoiceOfDarknessCharacter::GetBaseCharacterReferance_Implementation()
{
	IINT_BaseCharacter::GetBaseCharacterReferance_Implementation();

	return this;
}

bool AVoiceOfDarknessCharacter::IsPlayer_Implementation()
{
	 IINT_BaseCharacter::IsPlayer_Implementation();

	return IsPlayerControlled();
}

#pragma endregion
