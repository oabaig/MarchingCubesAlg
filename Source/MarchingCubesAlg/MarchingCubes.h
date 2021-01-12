// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkVoxelModeller.h"
#include "vtkSphereSource.h"
#include "vtkDataSetReader.h"
#include "vtkPointData.h"
#include "vtkPolyDataNormals.h"
#include "vtkDataArray.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "MarchingCubes.generated.h"

UCLASS()
class MARCHINGCUBESALG_API AMarchingCubes : public AActor
{
	GENERATED_BODY()

public:
	AMarchingCubes();

protected:
	void GenerateMeshs();
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* mesh;

public:
	UFUNCTION(BlueprintCallable) void MarchingCubes();
	UFUNCTION(BlueprintCallable) void SetFileName(FString filename);
	UFUNCTION(BlueprintCallable) void IsoValueRange(FString isoValueMinimum, FString isoValueMaximum, FString stepSize);
	UFUNCTION(BlueprintCallable) void DrawMesh();
	UFUNCTION(BlueprintCallable) void IncrementIsoValue(float newIsoValue);
	UFUNCTION(BlueprintCallable) void IncrementElement(float increment);

private:
	FString fileName;
	vtkSmartPointer<vtkPolyData> polyData;
	vtkSmartPointer<vtkDataSetReader> reader;
	vtkSmartPointer<vtkMarchingCubes> surface;
	int numElements;
	int isoCounter;
	double isoValue;
	double virtualIsoValue;
	double prevIsoValue;
	double isoValueMin;
	double isoValueMax;
	bool isoSet;
	double step;
	int currElement;
	int prevElement;
	struct MeshData {
		int element;
		double isoValue;
		TArray<int32> Triangles;
		TArray<FVector> vertices;
		TArray<FLinearColor> vertexColors;
		TArray<FVector> normals;
	};
	MeshData* meshData;
};
