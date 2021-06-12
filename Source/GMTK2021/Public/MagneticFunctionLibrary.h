// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MagneticFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class GMTK2021_API UMagneticFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * GetMagneticForce
	 * @brief	If both poles are small enough to be represented as single points then they can be considered to be point magnetic charges.
	 * @param ChargeA		Magnetic Charge of body A in ampere meter
	 * @param ChargeB		Magnetic Charge of body B in ampere meter
	 * @param Permeability	permeability of the intervening medium (SI unit: tesla meter per ampere, henry per meter or newton per ampere squared)
	 * @param Separation	distance between A and B  (SI unit: meter).
	 * @return				the intensity of the force between those two magnetic poles
	 */
	UFUNCTION(BlueprintPure)
	static float GetMagneticForce(float ChargeA, float ChargeB, float Permeability, float Separation);

};