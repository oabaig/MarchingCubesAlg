// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkVoxelModeller.h"
#include "vtkSphereSource.h"
#include "vtkImageData.h"
#include "vtkDicomImageReader.h"
#include "vtkDataSetReader.h"
#include "Misc/Paths.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "MarchingCubesPawn.generated.h"

UCLASS()
class MARCHINGCUBESALG_API AMarchingCubesPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMarchingCubesPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReadData();

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

};
