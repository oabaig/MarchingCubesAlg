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

	prevIsoValue = 0;
	prevElement = 0;

	isoValueMin = 0;
	isoValueMax = 10;
	isoSet = false;
	isoCounter = 0;
	isoValue = 0;
	virtualIsoValue = 0;
	currElement = 0;
	step = 0;
	elementIncrementSpeed = 1.0;

	//IsoValueRange("-3007", "725", "10.0");
	IsoValueRange("-3007", "725", "1.0");
}

void AMarchingCubes::GenerateMeshs()
{
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
	//vertexColors.Init(FLinearColor::Yellow, vertices.Num());
	//meshData[isoCounter].vertices = vertices;
	//meshData[isoCounter].vertexColors = vertexColors;

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
	//meshData[isoCounter].Triangles = Triangles;

	vtkDataArray* vtkNormalArray;
	vtkNormalArray = polyData->GetPointData()->GetNormals();
	
	
	double testDouble[3];
	for (int i = 0; i < vtkNormalArray->GetNumberOfTuples(); i++) {
		vtkNormalArray->GetTuple(i, testDouble);
		normals.Add(FVector(testDouble[0], testDouble[1], testDouble[2]));
	}
	//meshData[isoCounter].normals = normals;
}

void AMarchingCubes::DrawMesh() {
	prevElement = currElement;
	int i = currElement;
		
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		//TEXT("iso value: ") + FString::FromInt(i) + TEXT(" struct iso value: ") + FString::SanitizeFloat(meshData[i].isoValue));

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

void AMarchingCubes::IncrementElement(float increment)
{
	currElement += increment * elementIncrementSpeed;
	if (currElement < 0) {
		currElement = 0;
	}
	else if (currElement > numElements) {
		currElement = numElements;
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
	/*
	if (isoSet && !fileName.IsEmpty()) {
		for (isoCounter = 0; isoCounter < numElements + 1; isoCounter++) {
			meshData[isoCounter].isoValue = virtualIsoValue;
			meshData[isoCounter].element = isoCounter;

			polyData = vtkSmartPointer<vtkPolyData>::New();

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				//TEXT("iso value: ") + FString::SanitizeFloat(meshData[isoCounter].isoValue));

			surface->SetValue(0, virtualIsoValue);
			surface->Update();

			// gets iso value range. USE!!!
			//reader->GetOutput()->GetScalarRange();

			polyData = surface->GetOutput();

			GenerateMeshs();
			virtualIsoValue += step;
		}
		isoValue = 0;
		prevIsoValue = -1;
		DrawMesh();
	}*/

	if (isoSet && !fileName.IsEmpty()) {
		
		polyData = vtkSmartPointer<vtkPolyData>::New();
		//surface = vtkSmartPointer<vtkMarchingCubes>::New();
		surface->SetValue(0, isoValue);
		surface->Update();

		polyData = surface->GetOutput();

		GenerateMeshs();
		DrawMesh();
	}
}

void AMarchingCubes::SetFileName(FString filename)
{
	fileName = filename;
	const char* fname = TCHAR_TO_ANSI(*filename);
	reader->SetFileName(fname);
	reader->Update();
	surface->SetInputConnection(reader->GetOutputPort());
	surface->ComputeNormalsOn();
}

void AMarchingCubes::IsoValueRange(FString isoValueMinimum, FString isoValueMaximum, FString stepSize)
{
	isoValueMin = FCString::Atod(*isoValueMinimum);
	isoValueMax = FCString::Atod(*isoValueMaximum);
	step = FCString::Atod(*stepSize);

	numElements = (isoValueMax - isoValueMin) / step;

	virtualIsoValue = isoValueMin;
	isoSet = true;

	meshData = new MeshData [numElements + 1];
	
	meshData[0].element = 0;
	meshData[0].isoValue = isoValueMin;
	meshData[0].normals = TArray<FVector>();
	meshData[0].Triangles = TArray<int32>();
	meshData[0].vertexColors = TArray<FLinearColor>();
	meshData[0].vertices = TArray<FVector>();
}
