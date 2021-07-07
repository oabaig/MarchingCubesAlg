// Fill out your copyright notice in the Description page of Project Settings.


#include "TextAssetFactoryNew.h"

#include "TextAsset.h"

UTextAssetFactoryNew::UTextAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UTextAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


UObject* UTextAssetFactoryNew::FactoryCreateNew(UClass * InClass, UObject * InParent, FName InName, EObjectFlags Flags, UObject * Context, FFeedbackContext * Warn)
{
	return NewObject<UTextAsset>(InParent, InClass, InName, Flags);
}


bool UTextAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}