// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticComponent.h"

#include "MagneticFunctionLibrary.h"
#include "MagneticSubsystem.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

UMagneticComponent::UMagneticComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_StartPhysics;

	Charge = 10.f;
	bIsMonopole = true;

}

void UMagneticComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMagneticComponent::OnRegister()
{
	Super::OnRegister();
	if (auto MagnetSub = Cast<UMagneticSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UMagneticSubsystem::StaticClass())))
	{
		MagnetSub->RegisterMagneticComponent(this);
	}
}

void UMagneticComponent::OnUnregister()
{
	Super::OnUnregister();
	if (auto MagnetSub = Cast<UMagneticSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UMagneticSubsystem::StaticClass())))
	{
		MagnetSub->UnregisterMagneticComponent(this);
	}
}


// Called every frame
void UMagneticComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (TickType) {
	case ELevelTick::LEVELTICK_TimeOnly:
	case ELevelTick::LEVELTICK_All:
		{
			ApplyMagneticForce(DeltaTime);
		}
		break;
	case ELevelTick::LEVELTICK_ViewportsOnly:
	case ELevelTick::LEVELTICK_PauseTick:
		break;
	default: ;
	}
}

FVector UMagneticComponent::CalculateMagneticForce() const
{
	FVector FinalForce = FVector(0.f);
	TArray<UMagneticComponent*> Magnets;
	float Permeability = 1.f;
	if (auto MagnetSub = Cast<UMagneticSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(GetOwner(), UMagneticSubsystem::StaticClass())))
	{
		Magnets = MagnetSub->GetAllMagneticComponents();
		Permeability = MagnetSub->MagneticPermeability;
	}

	if (!Magnets.IsValidIndex(0))
		return FinalForce;

	for (auto MagnetItr : Magnets)
	{
		if (!MagnetItr || MagnetItr == this)
			continue;

		FVector AtoB = MagnetItr->GetComponentLocation() - GetComponentLocation();

		const auto APolarity= GetForwardVector();
		const auto BPolarity= MagnetItr->GetForwardVector();

		const auto AtoBNorm= AtoB.GetUnsafeNormal();

		const float MagneticForce =  UMagneticFunctionLibrary::GetMagneticForce(Charge, MagnetItr->Charge, Permeability, AtoB.Size()/100);

		if (bIsMonopole)
		{
			const bool A = IsPositive;
			const bool B =  MagnetItr->bIsMonopole ? MagnetItr->IsPositive : FVector::DotProduct(BPolarity.GetUnsafeNormal(), AtoBNorm) <= 0 ;
			const bool Attract = (!A != !B);
			FinalForce += MagneticForce * ( AtoBNorm.GetUnsafeNormal() * (Attract? 1.f: -1.f));
		}
		else
		{
			const float AttractA = FVector::DotProduct(APolarity.GetUnsafeNormal(), AtoBNorm);
			const float AttractB = FVector::DotProduct(BPolarity.GetUnsafeNormal(), AtoBNorm);
			const float Attract = AttractA * AttractB;
			FinalForce += (AtoB.GetUnsafeNormal() * Attract) * MagneticForce;
		}
	}
	return FinalForce;
}

void UMagneticComponent::ApplyMagneticForce(float DeltaTime)
{
	auto Force = CalculateMagneticForce();
	MagneticTick.Broadcast(Force, DeltaTime);
	LastFrameMagneticForce = Force;
}