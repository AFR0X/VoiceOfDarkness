// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetProgress.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

void UWidgetProgress::UpdateProgressBar(float CurrentValue, float MaxValue)
{
	float Value = UKismetMathLibrary::MapRangeClamped(CurrentValue, 0, MaxValue, 0, 1);
	ProgressBar->SetPercent(Value);
}
