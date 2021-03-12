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
	reader = vtkSmartPointer<vtkDataSetReader>::New();
	surface = vtkSmartPointer<vtkMarchingCubes>::New();

	// defualt scale
	SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/Game/VertexMat.VertexMat'"));
	Material = MaterialOb.Object;


	isoValueMin = 0;
	isoValueMax = 10;
	isoCounter = 0;
	isoValue = 0;
	elementIncrementSpeed = 1.0;
}

void AMarchingCubes::GenerateMesh()
{
	// empties all arrays
	Triangles.Empty();
	vertices.Empty();
	vertexColors.Empty();
	normals.Empty();

	mesh->ClearAllMeshSections();

	// output all points
	// and store all the points into a vector containing all the vertices
	double x[3];
	for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
	{
		polyData->GetPoint(i, x);
		vertices.Add(FVector(x[0], x[1], x[2]));
		vertexColors.Add(FLinearColor::Blue);
	}

	// create a cell array data to get all the polygons from the polygon data
	vtkSmartPointer<vtkCellArray> cellArray =
		vtkSmartPointer<vtkCellArray>::New();

	cellArray = polyData->GetPolys();

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

	vtkDataArray* vtkNormalArray;
	vtkNormalArray = polyData->GetPointData()->GetNormals();


	double testDouble[3];
	for (int i = 0; i < vtkNormalArray->GetNumberOfTuples(); i++) {
		vtkNormalArray->GetTuple(i, testDouble);
		normals.Add(FVector(testDouble[0], testDouble[1], testDouble[2]));
	}
}

void AMarchingCubes::DrawMesh() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		TEXT("iso value: ") + FString::FromInt(isoValue));

	// draws the vertices and triangles in Unreal
	// most of the fields are unused for the purpose of this project, so just create empty arrays
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals,
		TArray<FVector2D>(), vertexColors, TArray<FProcMeshTangent>(), true);
	mesh->SetMaterial(0, Material);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void AMarchingCubes::IncrementIsoValue(float newIsoValue)
{
	isoValue += newIsoValue * elementIncrementSpeed;
	if (isoValue > isoValueMax) {
		isoValue = isoValueMax;
	}
	else if (isoValue < isoValueMin) {
		isoValue = isoValueMin;
	}
}

void AMarchingCubes::IncreaseSpeed(int amount)
{
	elementIncrementSpeed = amount;
}

void AMarchingCubes::DecreaseSpeed(int amount)
{
	elementIncrementSpeed = amount;
}


void AMarchingCubes::MarchingCubes()
{
	polyData = vtkSmartPointer<vtkPolyData>::New();
	surface->SetValue(0, isoValue);
	surface->Update();

	polyData = surface->GetOutput();

	GenerateMesh();
	DrawMesh();
}

void AMarchingCubes::SetFileName(FString filename)
{
	const char* fname = TCHAR_TO_ANSI(*filename);
	reader->SetFileName(fname);
	reader->Update();
	surface->SetInputConnection(reader->GetOutputPort());
	surface->ComputeNormalsOn();

	double* scalarRange = reader->GetOutput()->GetScalarRange();
	IsoValueRange(scalarRange[0], scalarRange[1]);
}

void AMarchingCubes::IsoValueRange(int isoValueMinimum, int isoValueMaximum)
{
	isoValueMin = isoValueMinimum;
	isoValueMax = isoValueMaximum;
}
