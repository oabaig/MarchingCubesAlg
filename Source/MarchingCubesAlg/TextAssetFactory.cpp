// Fill out your copyright notice in the Description page of Project Settings.


#include "TextAssetFactory.h"

#include "Containers/UnrealString.h"
#include "TextAsset.h"
#include "Misc/FileHelper.h"

UTextAssetFactory::UTextAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("txt;")) + NSLOCTEXT("UTextAssetFactory", "FormatTxt", "Text File").ToString());
	SupportedClass = UTextAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}

UObject* UTextAssetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UTextAsset* TextAsset = nullptr;
	FString TextString;

	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		TextAsset = NewObject<UTextAsset>(InParent, InClass, InName, Flags);
		TextAsset->Text = FText::FromString(TextString);
	}

	bOutOperationCanceled = false;

	return TextAsset;
}
