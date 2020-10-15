// Fill out your copyright notice in the Description page of Project Settings.

#include "MarchingCubes.h"
#include "Engine.h"

// Sets default values
AMarchingCubes::AMarchingCubes()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

	polyData = vtkSmartPointer<vtkPolyData>::New();
}

void AMarchingCubes::GenerateMesh()
{
	// output number of points
	// unecessary
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		TEXT("Number of Points: ") + FString::SanitizeFloat(polyData->GetNumberOfPoints()));

	auto messageLog = FMessageLog("Points");
	messageLog.Open(EMessageSeverity::Info, true);

	// output all points
	// and store all the points into a vector containing all the vertices
	double x[3];
	TArray<FVector> vertices;
	for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
	{
		polyData->GetPoint(i, x);
		vertices.Add(FVector(x[0], x[1], x[2]));

		messageLog.Message(EMessageSeverity::Info,
			FText::FromString(FString::Printf(TEXT("Point %d: (%f, %f, %f)"), i, x[0], x[1], x[2])));
	}

	// create a cell array data to get all the polygons from the polygon data
	vtkSmartPointer<vtkCellArray> cellArray =
		vtkSmartPointer<vtkCellArray>::New();

	cellArray = polyData->GetPolys();

	// output number of triangles
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		TEXT("Number of Vertices: ") + FString::SanitizeFloat(cellArray->GetSize()));

	// create a list that stores the points, in order, for each triangle
	// get each vertice and add it into a triangles vector in order
	vtkSmartPointer<vtkIdList> p =
		vtkSmartPointer<vtkIdList>::New();
	int h;
	int numTriangles = 0;
	for (int i = 0; i < polyData->GetNumberOfPolys(); i++) {
		// GetNextCell returns 0 if end of cells, 1 if else
		h = cellArray->GetNextCell(p);
		if (h == 0) {
			break;
		}

		// if the number of vertices in p is 3
		if (p->GetNumberOfIds() == 3) {
			numTriangles++;
			Triangles.Add(p->GetId(0));
			Triangles.Add(p->GetId(1));
			Triangles.Add(p->GetId(2));
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		TEXT("Number of Triangles: ") + FString::FromInt(numTriangles));



	// draws the vertices and triangles in Unreal
	// most of the fields are unused for the purpose of this project, so just create empty arrays
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void AMarchingCubes::UnrealMarchingCubesTEST()
{
	vtkSmartPointer<vtkImageData> volume =
		vtkSmartPointer<vtkImageData>::New();
	double isoValue;

	
	// no argument case -- fix
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(500);
	sphereSource->SetPhiResolution(20);
	sphereSource->SetThetaResolution(20);
	sphereSource->Update();

	double bounds[6];
	sphereSource->GetOutput()->GetBounds(bounds);
	for (unsigned int i = 0; i < 6; i += 2) {
		double range = bounds[i + 1] - bounds[i];
		bounds[i] = bounds[i] - 0.1 * range;
		bounds[i + 1] = bounds[i + 1] + 0.1 * range;
	}
	vtkSmartPointer<vtkVoxelModeller> voxelModeller =
		vtkSmartPointer<vtkVoxelModeller>::New();
	voxelModeller->SetSampleDimensions(50, 50, 50);
	voxelModeller->SetModelBounds(bounds);
	voxelModeller->SetScalarTypeToFloat();
	voxelModeller->SetMaximumDistance(0.1);

	voxelModeller->SetInputConnection(sphereSource->GetOutputPort());
	voxelModeller->Update();
	isoValue = 0.5;
	volume->DeepCopy(voxelModeller->GetOutput());
	// end argument case 

	/*isoValue = 0.5;
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName("Test_Data_256x256");
	volume->DeepCopy(reader->GetOutput());
	reader->Update();*/
	/*
	vtkSmartPointer<vtkMarchingCubes> surface =
		vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData(volume);
	surface->ComputeNormalsOn();
	surface->SetValue(0, isoValue);
	surface->Update();

	polyData = surface->GetOutput();
	*/

	vtkSmartPointer<vtkDataSetReader> reader =
		vtkSmartPointer<vtkDataSetReader>::New();

	//reader->AddFileName("F:\\Unreal Projects\\MarchingCubesAlg\\bernard3D_Q.vtk");


	GenerateMesh();
}

void AMarchingCubes::PostActorCreated()
{
	Super::PostActorCreated();
	UnrealMarchingCubesTEST();
}

void AMarchingCubes::PostLoad()
{
	Super::PostLoad();
	UnrealMarchingCubesTEST();
}
