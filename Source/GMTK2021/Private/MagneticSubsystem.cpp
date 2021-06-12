// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticSubsystem.h"

UMagneticSubsystem::UMagneticSubsystem() : Super()
{
	MagneticPermeability = 1.f;
}

void UMagneticSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadConfig();
	Components.Empty();
}

void UMagneticSubsystem::Deinitialize()
{
	Components.Empty();
	Super::Deinitialize();
}

void UMagneticSubsystem::RegisterMagneticComponent(UMagneticComponent* Magnet)
{
	if (Magnet)
	{
		Components.Add(Magnet);
	}
}

void UMagneticSubsystem::UnregisterMagneticComponent(UMagneticComponent* Magnet)
{
	Components.Remove(Magnet);
}

TArray<UMagneticComponent*> UMagneticSubsystem::GetAllMagneticComponents() const
{
	return Components;
}