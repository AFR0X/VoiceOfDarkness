// Fill out your copyright notice in the Description page of Project Settings.


#include "A_MasterInteract.h"

// Sets default values
AA_MasterInteract::AA_MasterInteract()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AA_MasterInteract::BeginPlay()
{
	Super::BeginPlay();
	
}

void AA_MasterInteract::Interactable_Implementation(AVoiceOfDarknessCharacter* BaseCharacter)
{
	IINT_Interact::Interactable_Implementation(BaseCharacter);

	if (BaseCharacter)
	{
		
	}
}


