// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Flashlight.h"
#include "Components/SpotLightComponent.h"

// Sets default values
AA_Flashlight::AA_Flashlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	FlashlightMesh->SetupAttachment(GetRootComponent());
	FlashlightMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ConstructorHelpers::FObjectFinder<UStaticMesh> FlashlightFinder (TEXT("/Game/VoiceOfDarkness/Meshes/Flashlights_Pack/Meshes/SM_Tactical_Flashlight_B.SM_Tactical_Flashlight_B"));
    if (FlashlightFinder.Succeeded())
    {
	    FlashlightMesh->SetStaticMesh(FlashlightFinder.Object);
    }

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComp->SetupAttachment(FlashlightMesh, FName("Lamp"));
	SpotLightComp->IntensityUnits = ELightUnits::Lumens;
	SpotLightComp->Intensity = 500.0f;
	SpotLightComp->InnerConeAngle = 20.0f;
	SpotLightComp->OuterConeAngle = 40.0f;
	
	
	
}

// Called when the game starts or when spawned
void AA_Flashlight::BeginPlay()
{
	Super::BeginPlay();
	
}
