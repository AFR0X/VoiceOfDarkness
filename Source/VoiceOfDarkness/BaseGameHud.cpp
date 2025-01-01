// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameHud.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Game/BaseCharacter/WidgetGame.h"

ABaseGameHud::ABaseGameHud()
{
	ConstructorHelpers::FClassFinder<UWidgetGame> WidgetGameFinder (TEXT("/Game/VoiceOfDarkness/Widgets/Game/BaseCharacter/WB_Game"));
	if (WidgetGameFinder.Succeeded())
	{
		WidgetGameClass = WidgetGameFinder.Class;
	}
}

void ABaseGameHud::BeginPlay()
{
	Super::BeginPlay();

	if (!WidgetGameReferance)
	{
		WidgetGameReferance = CreateWidget<UWidgetGame>(GetOwner()->GetWorld(), WidgetGameClass);
		if (WidgetGameReferance)
		{
			WidgetGameReferance->AddToViewport(0);
		}
	}	
}

void ABaseGameHud::UpdateFlashlightWidget(float CurrentValue, float MaxValue)
{
	if (WidgetGameReferance)
	{
		WidgetGameReferance->UpdateFlashlightProgress(CurrentValue, MaxValue);
	}
}

void ABaseGameHud::UpdateSprintWidget(float CurrentValue, float MaxAmount)
{
	if (WidgetGameReferance)
	{
		WidgetGameReferance->UpdateSprintProgress(CurrentValue, MaxAmount);
	}
}

void ABaseGameHud::ViewInteractInfo(UInputAction* InteractInput, FText InteractText)
{
	if (WidgetGameReferance)
	{
		WidgetGameReferance->UpdateInteractInfo(InteractInput, InteractText);
	}
}

ABaseGameHud* ABaseGameHud::GetBaseGameHudReferance_Implementation()
{
	IINT_BaseGameHud::GetBaseGameHudReferance_Implementation();

	return this;
}



