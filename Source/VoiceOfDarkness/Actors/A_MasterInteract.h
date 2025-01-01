// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoiceOfDarkness/Interfaces/INT_Interact.h"
#include "A_MasterInteract.generated.h"

UCLASS()
class VOICEOFDARKNESS_API AA_MasterInteract : public AActor, public IINT_Interact
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_MasterInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	virtual void Interactable_Implementation(AVoiceOfDarknessCharacter* BaseCharacter) override;

};
