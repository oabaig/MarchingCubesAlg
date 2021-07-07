// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, hidecategories=(Object))
class MARCHINGCUBESALG_API UTextAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TextAsset")
		FText Text;
};
