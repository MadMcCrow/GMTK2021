// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MagneticSubsystem.generated.h"

/**
 *
 */
UCLASS(config=Game, defaultconfig)
class GMTK2021_API UMagneticSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UMagneticSubsystem();

	virtual void Initialize (FSubsystemCollectionBase & Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	float MagneticPermeability = 1.f;

	UFUNCTION(BlueprintCallable, Category="Magnet")
	void RegisterMagneticComponent(class UMagneticComponent* Magnet);

	UFUNCTION(BlueprintCallable, Category="Magnet")
	void UnregisterMagneticComponent(class UMagneticComponent* Magnet);

	UFUNCTION(BlueprintPure, Category="Magnet")
	TArray<class UMagneticComponent*> GetAllMagneticComponents() const;

private:

	UPROPERTY(Transient)
	TArray<class UMagneticComponent*> Components;
};