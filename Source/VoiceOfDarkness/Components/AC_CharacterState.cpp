// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_CharacterState.h"

#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VoiceOfDarkness/BaseGameHud.h"
#include "VoiceOfDarkness/VoiceOfDarknessCharacter.h"
#include "VoiceOfDarkness/Widgets/Game/BaseCharacter/WidgetGame.h"

// Sets default values for this component's properties
UAC_CharacterState::UAC_CharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	SpeedTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpeedTimeline"));

	ConstructorHelpers::FObjectFinder<UCurveFloat> SpeedCurveFinder (TEXT("/Script/Engine.CurveFloat'/Game/VoiceOfDarkness/Curves/Sprint_Curve.Sprint_Curve'"));
	if (SpeedCurveFinder.Succeeded())
	{
		SpeedCurve = SpeedCurveFinder.Object;
	}
	
}


// Called when the game starts
void UAC_CharacterState::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (SpeedCurve)
	{
		// Progress update için bind
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleSpeedProgress"));
		SpeedTimeline->AddInterpFloat(SpeedCurve, ProgressFunction);

		// Timeline bitiş eventi
		FOnTimelineEvent FinishEvent;
		FinishEvent.BindUFunction(this, FName("OnTimelineFinished"));
		SpeedTimeline->SetTimelineFinishedFunc(FinishEvent);
	}

	
}

#pragma region Sprint


void UAC_CharacterState::HandleSpeedProgress(float Value)
{
	BaseCharacterReferance->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(BaseSpeed, MaxSpeed, Value);
}

void UAC_CharacterState::OnTimelineFinished()
{
	UWidgetGame* WidgetGameReferance = BaseCharacterReferance->BaseGameHudReferance->WidgetGameReferance;
	if (SprintInfo.bIsSprint)
	{
		SprintTimerStart();

		if (WidgetGameReferance)
		{
			WidgetGameReferance->PlayAnimation(WidgetGameReferance->SprintAnim, 0.0f, 1, EUMGSequencePlayMode::Forward);
		}
	}
	else
	{
		if (WidgetGameReferance)
		{
			WidgetGameReferance->PlayAnimation(WidgetGameReferance->SprintAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		}
	}


}

void UAC_CharacterState::StartRunning()
{
	if (SprintInfo.SprintCurrentValue > 0 && !SprintInfo.bIsSprint && !BaseCharacterReferance->GetCapsuleComponent()->GetComponentVelocity().Equals(FVector(0,0,0),0))
	{
		SprintInfo.bIsSprint = true;
		SpeedTimeline->Play();
	}
}

void UAC_CharacterState::StopRunning()
{
	if (SprintInfo.bIsSprint)
	{
		SprintInfo.bIsSprint = false;
		SpeedTimeline->Reverse();
	}
}

void UAC_CharacterState::ControllingSprintTimer()
{
	if (!BaseCharacterReferance || !BaseCharacterReferance->BaseGameHudReferance)
	{
		UE_LOG(LogTemp, Error, TEXT("No Valid BaseGameHudRef or BaseCharacter (AC_CharacterState)"));
		return;
	}
	
	if (SprintInfo.bIsSprint)
	{
		if (BaseCharacterReferance->GetCapsuleComponent()->GetComponentVelocity().Equals(FVector(0,0,0),0))
		{
			StopRunning();
			SprintInfo.bIsSprint = false;
		}
		else
		{
			if (SprintInfo.SprintCurrentValue <= 0)
			{
				StopRunning();
				SprintInfo.bIsSprint = false;
			}
			else
			{
				SprintInfo.SprintCurrentValue -= SprintInfo.SprintDeleteAmount;
			}
		}
	}
	else
	{
		if (SprintInfo.SprintCurrentValue < SprintInfo.SprintMaxValue)
		{
			SprintInfo.SprintCurrentValue += FMath::Clamp(SprintInfo.SprintDeleteAmount, 0, 100);
		}
		else
		{
			SprintTimerClosed();
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Value %f"), SprintInfo.SprintCurrentValue);
	if (BaseCharacterReferance)
	{
		if (BaseCharacterReferance->BaseGameHudReferance)
		{
			BaseCharacterReferance->BaseGameHudReferance->UpdateSprintWidget(SprintInfo.SprintCurrentValue, SprintInfo.SprintMaxValue);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Valid BaseGameHudRef (AC_CharacterState)"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Valid CharacterReferance (AC_CharacterState)"));
	}
}

void UAC_CharacterState::SprintTimerStart()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (!World->GetTimerManager().IsTimerActive(SprintTimerHandle))
		{
			World->GetTimerManager().SetTimer(SprintTimerHandle, this, &UAC_CharacterState::ControllingSprintTimer, SprintInfo.SprintTimerDuration, true);
			SprintInfo.bIsSprint = true;
		}
	}
}

void UAC_CharacterState::SprintTimerClosed()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (World->GetTimerManager().IsTimerActive(SprintTimerHandle))
		{
			World->GetTimerManager().ClearTimer(SprintTimerHandle);

			if (BaseCharacterReferance)
			{
				StopRunning();
				SprintInfo.bIsSprint = false;
			}
		}
	}
}

void UAC_CharacterState::InitCharacterState(AVoiceOfDarknessCharacter* CharacterRef)
{
	BaseCharacterReferance = CharacterRef;
	if (BaseCharacterReferance)
	{
		if (BaseCharacterReferance->BaseGameHudReferance)
		{
			BaseCharacterReferance->BaseGameHudReferance->UpdateSprintWidget(SprintInfo.SprintCurrentValue, SprintInfo.SprintMaxValue);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error No Valid BaseGameHud (AC_CharacterState)"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Valid BaseCharacter (AC_CharacterState)"));
	}
}


#pragma endregion
