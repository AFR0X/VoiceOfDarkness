// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Flashlight.generated.h"

class USpotLightComponent;

UCLASS()
class VOICEOFDARKNESS_API AA_Flashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_Flashlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* FlashlightMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* SpotLightComp;
	
	
public:	

};
