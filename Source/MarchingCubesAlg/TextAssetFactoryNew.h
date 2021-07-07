// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "TextAssetFactoryNew.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object)
class MARCHINGCUBESALG_API UTextAssetFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	bool ShouldShowInNewMenu() const;
};