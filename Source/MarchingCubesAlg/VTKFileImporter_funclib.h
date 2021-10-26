// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "vtkSmartPointer.h"
#include "vtkDataSetReader.h"
#include "vtkImageMapToColors.h"
#include "vtkJPEGWriter.h"
#include "vtkJPEGReader.h"
#include "vtkImageAppend.h"
#include "vtkLookupTable.h"
#include "vtkExtractVOI.h"
#include "vtkImageActor.h"

#include "vtkPNGWriter.h"
#include "vtkPNGReader.h"
#include "vtkImageData.h"
#include "vtkImageFlip.h"

#include "Runtime\Core\Public\Misc\Paths.h"

#include <sstream>

#include "VTKFileImporter_funclib.generated.h"

/**
 * Imports a vtk file and converts it into a 2d sliced map
 */
UCLASS()
class MARCHINGCUBESALG_API UVTKFileImporter_funclib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="SliceImages") static void SliceImages(FString fname);
	
};
