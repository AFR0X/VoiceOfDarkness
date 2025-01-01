// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoiceOfDarkness/Actors/A_MasterInteract.h"
#include "A_Child_ItemSMesh.generated.h"

class USceneComponent;
class USphereComponent;
class UInputAction;

/**
 * 
 */
UCLASS()
class VOICEOFDARKNESS_API AA_Child_ItemSMesh : public AA_MasterInteract
{
	GENERATED_BODY()

public:

	AA_Child_ItemSMesh();
	
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> RootSceneComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FDataTableRowHandle ItemDataRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInfo")
	UInputAction* InteractInput = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInfo")
	FText InteractText = FText::FromString("Interact");
	
	// BeginOverlap ve EndOverlap fonksiyonları
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
							  AActor* OtherActor, 
							  UPrimitiveComponent* OtherComp, 
							  int32 OtherBodyIndex, 
							  bool bFromSweep, 
							  const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, 
							AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, 
							int32 OtherBodyIndex);

	
	virtual void Interactable_Implementation(AVoiceOfDarknessCharacter* BaseCharacter) override;

public:

	UFUNCTION(CallInEditor, Category = "Inventory")
	void UpdateItem();
	

	
	
};
