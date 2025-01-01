// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "AC_CharacterState.generated.h"

class AVoiceOfDarknessCharacter;
class UTimelineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEOFDARKNESS_API UAC_CharacterState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_CharacterState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	AVoiceOfDarknessCharacter* BaseCharacterReferance = nullptr;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
#pragma region Sprint

protected:
	
	UPROPERTY()
	UTimelineComponent* SpeedTimeline;
	UPROPERTY()
	UCurveFloat* SpeedCurve;

	// Koşma hızları
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseSpeed = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 600.0f;
	
	// Timeline Event Fonksiyonları
	UFUNCTION()
	void HandleSpeedProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();
	
public:
	
	// Koşma input fonksiyonları
	void StartRunning();
	void StopRunning();
	
	void SprintTimerStart();
	void SprintTimerClosed();

	void InitCharacterState(AVoiceOfDarknessCharacter* CharacterRef);
	
protected:
	
	UFUNCTION()
	void ControllingSprintTimer();

	
	
public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterInfo")
	FTimerHandle SprintTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterInfo")
	FSprintInfo SprintInfo;
	
#pragma endregion
};
