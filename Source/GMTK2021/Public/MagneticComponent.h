// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "MagneticComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMagneticPhysicTick, FVector, MagneticForce, float, DeltaTime);

UCLASS( BlueprintType, Blueprintable, ClassGroup=(Magnetic), meta=(BlueprintSpawnableComponent) )
class GMTK2021_API UMagneticComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMagneticComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(BlueprintAssignable, blueprintReadWrite, Category= "Magnetic")
	FMagneticPhysicTick MagneticTick;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetic")
	float Charge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetic")
	bool bIsMonopole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetic", meta=(EditCondition = "bIsMonopole"))
	bool IsPositive;

	virtual FVector CalculateMagneticForce() const;

public:

	UFUNCTION(BlueprintPure, Category = "Magnetic")
	FVector GetMagneticForce() const {return  LastFrameMagneticForce;}

protected:

	virtual void ApplyMagneticForce(float DeltaTime);

private:
	UPROPERTY(Transient)
	FVector LastFrameMagneticForce;




};