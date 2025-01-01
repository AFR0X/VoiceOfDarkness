// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Child_ItemSMesh.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceOfDarkness/BaseGameHud.h"
#include "VoiceOfDarkness/BaseStructures.h"
#include "VoiceOfDarkness/VoiceOfDarknessCharacter.h"
#include "VoiceOfDarkness/Components/AC_InventoryComponent.h"
#include "VoiceOfDarkness/Interfaces/INT_BaseCharacter.h"
#include "VoiceOfDarkness/Interfaces/INT_BaseGameHud.h"

AA_Child_ItemSMesh::AA_Child_ItemSMesh()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComp"));
	RootSceneComp->SetupAttachment(GetRootComponent());

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemStaticMesh->SetupAttachment(RootSceneComp);
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemStaticMesh->SetCollisionObjectType(ECC_GameTraceChannel1);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetupAttachment(ItemStaticMesh);
	
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder (TEXT("/Script/Engine.DataTable'/Game/VoiceOfDarkness/DataTables/DT_ItemSMesh.DT_ItemSMesh'"));
	if (DataTableFinder.Succeeded())
	{
		ItemDataRowHandle.DataTable = DataTableFinder.Object;
		ItemDataRowHandle.RowName = DataTableFinder.Object->GetRowNames()[0];
	}
}

void AA_Child_ItemSMesh::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AA_Child_ItemSMesh::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AA_Child_ItemSMesh::OnSphereEndOverlap);
}

void AA_Child_ItemSMesh::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor null kontrolü
	if (!OtherActor || !OtherActor->GetClass()->ImplementsInterface(UINT_BaseCharacter::StaticClass()))
	{
		return;
	}

	// Oyuncu olup olmadığını kontrol ediyoruz
	if (!IINT_BaseCharacter::Execute_IsPlayer(OtherActor))
	{
		return;
	}

	// HUD referansı alınıyor
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;
	}

	ABaseGameHud* BaseGameHudRef = Cast<ABaseGameHud>(PlayerController->GetHUD());
	if (!BaseGameHudRef)
	{
		return;
	}

	// Interact bilgilerini HUD'a gönderiyoruz
	if (InteractInput && !InteractText.IsEmpty())
	{
		BaseGameHudRef->ViewInteractInfo(InteractInput, InteractText);
	}
}

void AA_Child_ItemSMesh::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// OtherActor null kontrolü
	if (!OtherActor || !OtherActor->GetClass()->ImplementsInterface(UINT_BaseCharacter::StaticClass()))
	{
		return;
	}

	// Oyuncu olup olmadığını kontrol ediyoruz
	if (!IINT_BaseCharacter::Execute_IsPlayer(OtherActor))
	{
		return;
	}

	// HUD referansı alınıyor
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;
	}

	ABaseGameHud* BaseGameHudRef = Cast<ABaseGameHud>(PlayerController->GetHUD());
	if (!BaseGameHudRef)
	{
		return;
	}

	BaseGameHudRef->ViewInteractInfo(nullptr, InteractText);
}

void AA_Child_ItemSMesh::Interactable_Implementation(AVoiceOfDarknessCharacter* BaseCharacter)
{
	Super::Interactable_Implementation(BaseCharacter);

	if (BaseCharacter)
	{
		FInventoryInfoStruct* Item = ItemDataRowHandle.DataTable->FindRow<FInventoryInfoStruct>(ItemDataRowHandle.RowName, "INVENTORY", true);
		if (Item)
		{
			if (BaseCharacter->GetInventoryReferance()->AddInventoryItem(*Item))
			{
				Destroy();
			}
		}
	}
}

void AA_Child_ItemSMesh::UpdateItem()
{
	if (ItemDataRowHandle.DataTable)
	{
		FInventoryInfoStruct* ItemData = ItemDataRowHandle.DataTable->FindRow<FInventoryInfoStruct>(ItemDataRowHandle.RowName, "INVENTORY", true);
		if (ItemData->ItemStaticMesh)
		{
			ItemStaticMesh->SetStaticMesh(ItemData->ItemStaticMesh);
			ItemData->ActorClass = this->GetClass();
		}
	}
}
