#pragma once
#include "BaseStructures.generated.h"

USTRUCT(Blueprintable)
struct FFlashlightInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	float CurrentAmount = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	float MaxAmount = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	FName AttackSocket = FName("S_Flashlight");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	float DeleteAmount = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	float FlashlightTimerDuration = 0.5f;
};


USTRUCT(Blueprintable)
struct FSprintInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintCurrentValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintMaxValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	bool bIsSprint = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float MinimumSprint = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float MaximumSprint = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintTimerDuration = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintDeleteAmount = 1.f;
	
};

USTRUCT(Blueprintable)
struct FInventoryInfoStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemId = FString::Printf(TEXT("None"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText ItemName = FText::FromString("None");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText ItemDescription = FText::FromString("None");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemTexture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bIsStacktable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 ItemAmount = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AActor> ActorClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UStaticMesh* ItemStaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	USkeletalMesh* ItemSkeletalMesh = nullptr;
	
};
