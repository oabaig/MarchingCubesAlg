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
	// Generates the Mesh
	void GenerateMesh();
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* mesh;
	// Material of object
	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
		UMaterial* Material;


public:
	UFUNCTION(BlueprintCallable) void MarchingCubes();
	UFUNCTION(BlueprintCallable) void SetFileName(FString filename); // Sets file name
	UFUNCTION(BlueprintCallable) void IsoValueRange(int isoValueMinimum, int isoValueMaximum); // Gets the range of iso values
	UFUNCTION(BlueprintCallable) void DrawMesh(); // Draws mesh in space
	UFUNCTION(BlueprintCallable) void IncrementIsoValue(float newIsoValue); // increments iso value
	UFUNCTION(BlueprintCallable) void IncreaseSpeed(int amount); // increases increment speed
	UFUNCTION(BlueprintCallable) void DecreaseSpeed(int amount); // decreases increment speed

private:
	vtkSmartPointer<vtkPolyData> polyData;
	vtkSmartPointer<vtkDataSetReader> reader;
	vtkSmartPointer<vtkMarchingCubes> surface;
	int numElements;
	int isoCounter;
	double isoValue;
	double isoValueMin;
	double isoValueMax;
	int elementIncrementSpeed;
	int currElement;
	TArray<int32> Triangles;
	TArray<FVector> vertices;
	TArray<FLinearColor> vertexColors;
	TArray<FVector> normals;
};
