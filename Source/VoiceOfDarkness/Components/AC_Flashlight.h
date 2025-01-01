// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "AC_Flashlight.generated.h"

class AVoiceOfDarknessCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEOFDARKNESS_API UAC_Flashlight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Flashlight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION()
	void InitFlashlightComponent(AVoiceOfDarknessCharacter* CharacterRef);

	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	AVoiceOfDarknessCharacter* BaseCharacterReferance = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	TSubclassOf<AActor> FlashlightSpawnClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flashlight")
	AActor* FlashlightReferance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	FFlashlightInfo FlashlightInfo;


	// Functions


	UFUNCTION()
	void FlashlightToggleSystem();
	
	void FlashlightOpen();
	void FlashlightClosed();

	UPROPERTY()
	FTimerHandle FlashlightTimerHandle;
	UFUNCTION()
	void FlashlightControlling();
	
};
