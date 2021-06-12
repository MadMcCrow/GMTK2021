// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticFunctionLibrary.h"

float UMagneticFunctionLibrary::GetMagneticForce(float ChargeA, float ChargeB, float Permeability, float Separation)
{
	return (Permeability *  ChargeA * ChargeB) / (4.f * PI * (Separation * Separation));
}