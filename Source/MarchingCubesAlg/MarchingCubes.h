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
	// Sets default values for this actor's properties
	AMarchingCubes();

protected:
	void GenerateMesh();
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* mesh;

public:
	UFUNCTION(BlueprintCallable) void MarchingCubes(FString filename, FString isoValueStr);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh parameters")
		FVector CubeRadius = FVector(100.0f, 100.0f, 100.0f);

private:
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	TArray<int32> Triangles;
	vtkSmartPointer<vtkPolyData> polyData;

};
