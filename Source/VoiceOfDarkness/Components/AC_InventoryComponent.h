// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "AC_InventoryComponent.generated.h"

class UWidgetInventory;
class AVoiceOfDarknessCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEOFDARKNESS_API UAC_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_InventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetInventory")
	TSubclassOf<UWidgetInventory> WidgetInventoryClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UAnimSequenceBase* BackPackBegin = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UAnimSequenceBase* BackPackEnd = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UAnimMontage* InventoryOpenAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UAnimMontage* InventoryCloseAnim = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FTimerHandle AnimTimerHandle;
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	AVoiceOfDarknessCharacter* BaseCharacterReferance = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetInventory")
	UWidgetInventory* WidgetInventoryReferance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryInfoStruct> InventoryData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 CurrentSlotSize = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSlotSize = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UAnimMontage* InventoryLoopAnim = nullptr;
	// Functions

	UFUNCTION()
	void InitInventoryComp(AVoiceOfDarknessCharacter* CharacterRef);
	
	UFUNCTION()
	void PressedInventory();
	
	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	bool AddInventoryItem(FInventoryInfoStruct Item);
		
};
