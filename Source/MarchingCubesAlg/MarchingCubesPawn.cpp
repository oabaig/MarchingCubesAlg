// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesPawn.h"
#include "Engine.h"

// Sets default values
AMarchingCubesPawn::AMarchingCubesPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	//OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMarchingCubesPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarchingCubesPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMarchingCubesPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMarchingCubesPawn::ReadData);
}

void AMarchingCubesPawn::ReadData()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		TEXT("DONE!!!"));
	vtkSmartPointer<vtkDataSetReader> reader =
		vtkSmartPointer<vtkDataSetReader>::New();

	FString directory = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"));
	FString result;
	FString myFile = directory + TEXT("/test.vtk");

	const char* fileName = TCHAR_TO_ANSI(*myFile);

	reader->SetFileName(fileName);
	reader->Update();

	//reader->CloseVTKFile();
	/*
	FString directory = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"));
	FString result;
	FString myFile = directory + TEXT("/test.txt");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		myFile);
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.CreateDirectory(*directory)) {
		bool test = FFileHelper::LoadFileToString(result, *myFile);
		if (test) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				TEXT("Works"));
		}
		else if (!test) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				TEXT("Nope"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("Nope2"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		result);
		*/
	/*
	FString directory = FPaths::Combine(FPaths::ProjectContentDir(), "Data");
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if ("file://C://Dev//UnrealProjects//MarchingCubesAlg//Content//Data//test.txt") {
		FString myFile = directory + "/" + "bernard3D_Q.txt";
		bool test = FFileHelper::LoadFileToString(result, *myFile);
		if (test) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				TEXT("Works"));
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("test2"));
	}

	*/


}

