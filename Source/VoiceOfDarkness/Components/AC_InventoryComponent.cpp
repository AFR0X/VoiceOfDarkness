// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "VoiceOfDarkness/VoiceOfDarknessCharacter.h"
#include "VoiceOfDarkness/Widgets/Game/Inventory/WidgetInventory.h"

// Sets default values for this component's properties
UAC_InventoryComponent::UAC_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	ConstructorHelpers::FClassFinder<UWidgetInventory> WidgetInventoryFinder (TEXT("/Game/VoiceOfDarkness/Widgets/Game/Inventory/WB_Inventory"));
	if (WidgetInventoryFinder.Succeeded())
	{
		WidgetInventoryClass = WidgetInventoryFinder.Class;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimOpenFinder (TEXT("/Script/Engine.AnimMontage'/Game/VoiceOfDarkness/BaseCharacter/Animations/Sequences/General/A_Backpack_Begin_Montage.A_Backpack_Begin_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimEndFinder (TEXT("/Script/Engine.AnimMontage'/Game/VoiceOfDarkness/BaseCharacter/Animations/Sequences/General/A_BackPack_End_Montage.A_BackPack_End_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimLoopFinder (TEXT("/Script/Engine.AnimMontage'/Game/VoiceOfDarkness/BaseCharacter/Animations/Sequences/General/A_BackPack_Loop_Montage.A_BackPack_Loop_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimSequence> AnimBackpackBeginFinder (TEXT("/Script/Engine.AnimSequence'/Game/VoiceOfDarkness/Meshes/Backpack/Anims/A_Loot_Backpack_Begin.A_Loot_Backpack_Begin'"));
	ConstructorHelpers::FObjectFinder<UAnimSequence> AnimBackpackEndFinder (TEXT("/Script/Engine.AnimSequence'/Game/VoiceOfDarkness/Meshes/Backpack/Anims/A_Loot_Backpack_End.A_Loot_Backpack_End'"));
	if (AnimOpenFinder.Succeeded() && AnimEndFinder.Succeeded() && AnimLoopFinder.Succeeded() && AnimBackpackBeginFinder.Succeeded() && AnimBackpackEndFinder.Succeeded())
	{
		InventoryOpenAnim = AnimOpenFinder.Object;
		InventoryCloseAnim = AnimEndFinder.Object;
		InventoryLoopAnim = AnimLoopFinder.Object;
		BackPackBegin = AnimBackpackBeginFinder.Object;
		BackPackEnd = AnimBackpackEndFinder.Object;
	}
}


// Called when the game starts
void UAC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	
	
}

void UAC_InventoryComponent::InitInventoryComp(AVoiceOfDarknessCharacter* CharacterRef)
{
	BaseCharacterReferance = CharacterRef;
	
	InventoryData.SetNum(MaxSlotSize);
}

void UAC_InventoryComponent::PressedInventory()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AnimTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation is currently playing. Please wait. 2"));
		return;
	}
	
	if (!BaseCharacterReferance)
	{
		return;
	}

	UAnimInstance* AnimInstance = BaseCharacterReferance->GetMesh()->GetAnimInstance();
	if (AnimInstance && (AnimInstance->Montage_IsPlaying(InventoryOpenAnim) || AnimInstance->Montage_IsPlaying(InventoryCloseAnim)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation is currently playing. Please wait."));
		return;
	}
	
	const bool bIsInventoryOpen = (WidgetInventoryReferance != nullptr);
	BaseCharacterReferance->GetBackPackReferance()->PlayAnimation(bIsInventoryOpen ? BackPackEnd : BackPackBegin, false);

	UAnimMontage* SelectedAnim = bIsInventoryOpen ? InventoryCloseAnim : InventoryOpenAnim;
	float DurationAnim = BaseCharacterReferance->PlayAnimMontage(SelectedAnim);

	if (DurationAnim > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &UAC_InventoryComponent::ToggleInventory, DurationAnim, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to play animation montage."));
	}
}

void UAC_InventoryComponent::ToggleInventory()
{
	if (!WidgetInventoryClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetInventoryClass is null! Cannot create inventory widget."));
		return;
	}
	
	if (WidgetInventoryReferance)
	{
		if (WidgetInventoryReferance->IsInViewport())
		{
			WidgetInventoryReferance->RemoveFromParent();
			WidgetInventoryReferance = nullptr;
		}
		else
		{
			WidgetInventoryReferance->AddToViewport();
			WidgetInventoryReferance->InitInventory(this);
		}
	}
	else
	{
		if (UWorld* World = GetOwner()->GetWorld())
		{
			WidgetInventoryReferance = CreateWidget<UWidgetInventory>(World, WidgetInventoryClass);
			if (WidgetInventoryReferance)
			{
				WidgetInventoryReferance->InitInventory(this);
				UE_LOG(LogTemp, Log, TEXT("Inventory widget created and initialized successfully."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create WidgetInventoryReferance!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("World is null! Cannot create inventory widget."));
		}

		if (WidgetInventoryReferance)
		{
			WidgetInventoryReferance->AddToViewport();
		}
	}
}

void UAC_InventoryComponent::RefreshInventory()
{
	if (WidgetInventoryReferance)
	{
		WidgetInventoryReferance->InitInventory(this);
	}
}

bool UAC_InventoryComponent::AddInventoryItem(FInventoryInfoStruct Item)
{
	// Null veya geçersiz item kontrolü
	if (!Item.ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item passed to AddInventoryItem"));
		return false;
	}

	bool bIsFound = false;
	int32 FoundIndex = INDEX_NONE;
	int32 EmptySlotIndex = INDEX_NONE;

	// Envanterde mevcut itemi ya da boş bir slotu bulmak için döngü
	for (int32 i = 0; i < CurrentSlotSize; ++i)
	{
		FInventoryInfoStruct& ItemData = InventoryData[i];

		if (ItemData.ActorClass == nullptr && EmptySlotIndex == INDEX_NONE)
		{
			EmptySlotIndex = i;
		}

		// Eşleşen bir item varsa işlem yapıyoruz
		if (ItemData.ItemId == Item.ItemId)
		{
			// Eğer stacklenebilir ise miktarı artır
			if (Item.bIsStacktable)
			{
				bIsFound = true;
				FoundIndex = i;
				break;
			}
		}
	}


	if (bIsFound)
	{
		InventoryData[FoundIndex].ItemAmount += Item.ItemAmount;
		RefreshInventory();
		return true;
	}
	else
	{
		// Eğer boş bir slot varsa oraya yeni item eklenir
		if (EmptySlotIndex != INDEX_NONE)
		{
			InventoryData[EmptySlotIndex] = Item;
			RefreshInventory();
			UE_LOG(LogTemp, Log, TEXT("Item added to slot %d"), EmptySlotIndex);

			return  true;
		}
		else
		{
			// Envanter dolu hatası
			UE_LOG(LogTemp, Error, TEXT("Inventory is full, unable to add item."));
		}
	}
	

	return false;
}


