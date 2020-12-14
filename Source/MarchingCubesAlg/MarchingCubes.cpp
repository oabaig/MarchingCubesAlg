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
	
	// defualt scale
	SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));
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
	TArray<FLinearColor> vertexColors;
	for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
	{
		polyData->GetPoint(i, x);
		vertices.Add(FVector(x[0], x[1], x[2]));
		vertexColors.Add(FLinearColor(FColor::White));

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

	vtkDataArray* vtkNormalArray;
	vtkNormalArray = polyData->GetPointData()->GetNormals();
	TArray<FVector> normals;
	TArray<FVector> tangents;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		TEXT("Number of Tuples: ") + FString::FromInt(vtkNormalArray->GetNumberOfTuples()));
	double testDouble[3];
	for (int i = 0; i < vtkNormalArray->GetNumberOfTuples(); i++) {
		vtkNormalArray->GetTuple(i, testDouble);
		normals.Add(FVector(testDouble[0], testDouble[1], testDouble[2]));
	}

	// draws the vertices and triangles in Unreal
	// most of the fields are unused for the purpose of this project, so just create empty arrays
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, TArray<FVector2D>(), vertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);

	
}

void AMarchingCubes::MarchingCubes(FString filename, FString isoValueStr)
{	
	double isoValue = FCString::Atod(*isoValueStr);
	FString directory = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"), filename);
	const char* fname = TCHAR_TO_ANSI(*directory);
	vtkSmartPointer<vtkDataSetReader> reader =
		vtkSmartPointer<vtkDataSetReader>::New();

	reader->SetFileName(fname);
	reader->Update();
	
	vtkSmartPointer<vtkMarchingCubes> surface =
		vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputConnection(reader->GetOutputPort());
	surface->ComputeNormalsOn();
	surface->SetValue(0, isoValue);
	surface->Update();
	
	polyData = surface->GetOutput();

	GenerateMesh();

}

void AMarchingCubes::PostActorCreated()
{
	Super::PostActorCreated();
	//UnrealMarchingCubesTEST();
}

void AMarchingCubes::PostLoad()
{
	Super::PostLoad();
	//UnrealMarchingCubesTEST();
}
