// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Flashlight.h"

#include "VoiceOfDarkness/BaseGameHud.h"
#include "VoiceOfDarkness/VoiceOfDarknessCharacter.h"
#include "VoiceOfDarkness/Actors/Flashlight/A_Flashlight.h"
#include "VoiceOfDarkness/Widgets/Game/BaseCharacter/WidgetGame.h"

// Sets default values for this component's properties
UAC_Flashlight::UAC_Flashlight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	ConstructorHelpers::FClassFinder<AA_Flashlight> FlashlightClassFinder (TEXT("/Script/VoiceOfDarkness.A_Flashlight"));
	if (FlashlightClassFinder.Succeeded())
	{
		FlashlightSpawnClass = FlashlightClassFinder.Class;
	}
}


// Called when the game starts
void UAC_Flashlight::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_Flashlight::InitFlashlightComponent(AVoiceOfDarknessCharacter* CharacterRef)
{
	BaseCharacterReferance = CharacterRef;
	if (BaseCharacterReferance)
	{
		if (BaseCharacterReferance->BaseGameHudReferance)
		{
			BaseCharacterReferance->BaseGameHudReferance->UpdateFlashlightWidget(FlashlightInfo.CurrentAmount, FlashlightInfo.MaxAmount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error No Valid BaseGameHud (AC_Flashlight)"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Valid BaseCharacter (AC_CharacterState)"));
	}
}

void UAC_Flashlight::FlashlightToggleSystem()
{
	if (FlashlightReferance)
	{
		FlashlightClosed();
	}
	else
	{
		FlashlightOpen();
	}
}

void UAC_Flashlight::FlashlightOpen()
{
	if (FlashlightInfo.CurrentAmount <= 0 || FlashlightReferance)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Valid Flashlight Amount"));
		return;
	}

	
	UWidgetGame* WidgetGameReferance = BaseCharacterReferance->BaseGameHudReferance->WidgetGameReferance;
	ABaseGameHud* BaseGameHudReferance = BaseCharacterReferance->BaseGameHudReferance;

	if (!WidgetGameReferance || !BaseGameHudReferance || !BaseCharacterReferance)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR NO VALID WidgetGameRef OR BaseGameHud Referance (AC_Flashlight)"));
		return;
	}
	
	
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null! Cannot spawn flashlight."));
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FlashlightReferance = World->SpawnActor<AActor>(FlashlightSpawnClass, SpawnParameters);
	if (FlashlightReferance)
	{
		FlashlightReferance->SetActorEnableCollision(false);
		if (BaseCharacterReferance->GetMesh()->DoesSocketExist(FlashlightInfo.AttackSocket))
		{
			FlashlightReferance->AttachToComponent(BaseCharacterReferance->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FlashlightInfo.AttackSocket);
			BaseCharacterReferance->ChangeAnimationSystem(EAnimationCategory::FLASHLIGHT);
			
			WidgetGameReferance->PlayAnimation(WidgetGameReferance->FlashlightAnim, 0.0f, 1, EUMGSequencePlayMode::Forward);
			UE_LOG(LogTemp, Log, TEXT("Flashlight successfully spawned and attached to socket %s."), *FlashlightInfo.AttackSocket.ToString());

			if (!World->GetTimerManager().IsTimerActive(FlashlightTimerHandle))
			{
				World->GetTimerManager().SetTimer(FlashlightTimerHandle, this, &UAC_Flashlight::FlashlightControlling,  FlashlightInfo.FlashlightTimerDuration, true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket %s does not exist on character mesh!"), *FlashlightInfo.AttackSocket.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn flashlight actor."));
	}
	
}

void UAC_Flashlight::FlashlightClosed()
{
	UWidgetGame* WidgetGameReferance = BaseCharacterReferance->BaseGameHudReferance->WidgetGameReferance;
	ABaseGameHud* BaseGameHudReferance = BaseCharacterReferance->BaseGameHudReferance;
	
	if (!BaseCharacterReferance || !FlashlightReferance || !BaseGameHudReferance || !WidgetGameReferance)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacterReferance or FlashlightSpawnClass is null! Cannot destroy flashlight. (AC_Flashlight)"));
		return;
	}

	BaseCharacterReferance->ChangeAnimationSystem(EAnimationCategory::NO_EQUIP);
	WidgetGameReferance->PlayAnimation(WidgetGameReferance->FlashlightAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
	FlashlightReferance->Destroy();
	FlashlightReferance = nullptr;

	UWorld* World = GetWorld();
	if (World)
	{
		if (World->GetTimerManager().IsTimerActive(FlashlightTimerHandle))
		{
			World->GetTimerManager().ClearTimer(FlashlightTimerHandle);
		}
	}
}

void UAC_Flashlight::FlashlightControlling()
{
	if (FlashlightInfo.CurrentAmount <= 0)
	{
		FlashlightClosed();
	}
	else
	{
		FlashlightInfo.CurrentAmount -= FlashlightInfo.DeleteAmount;
		BaseCharacterReferance->BaseGameHudReferance->UpdateFlashlightWidget(FlashlightInfo.CurrentAmount, FlashlightInfo.MaxAmount);
	}
}

