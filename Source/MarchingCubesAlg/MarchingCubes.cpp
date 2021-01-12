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

	//IsoValueRange("-3007", "725", "10.0");
	IsoValueRange("-3007", "725", "1.0");
}

void AMarchingCubes::GenerateMeshs()
{
	TArray<int32> Triangles;
	TArray<FVector> vertices;
	TArray<FLinearColor> vertexColors;
	TArray<FVector> normals;

	// output number of points
	//auto messageLog = FMessageLog("Points");
	//messageLog.Open(EMessageSeverity::Info, true);

	// output all points
	// and store all the points into a vector containing all the vertices
	double x[3];
	for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
	{
		polyData->GetPoint(i, x);
		vertices.Add(FVector(x[0], x[1], x[2]));
		vertexColors.Add(FLinearColor(FColor::White));

		//messageLog.Message(EMessageSeverity::Info,
		//	FText::FromString(FString::Printf(TEXT("Point %d: (%f, %f, %f)"), i, x[0], x[1], x[2])));
	}
	meshData[isoCounter].vertices = vertices;
	meshData[isoCounter].vertexColors = vertexColors;

	// create a cell array data to get all the polygons from the polygon data
	vtkSmartPointer<vtkCellArray> cellArray =
		vtkSmartPointer<vtkCellArray>::New();

	cellArray = polyData->GetPolys();

	// output number of triangles
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	//	TEXT("Number of Vertices: ") + FString::SanitizeFloat(cellArray->GetSize()));

	// create a list that stores the points, in order, for each triangle
	// get each vertice and add it into a triangles vector in order
	vtkSmartPointer<vtkIdList> p =
		vtkSmartPointer<vtkIdList>::New();
	int h;
	int numTriangles = 0;
	Triangles.Empty();
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
	meshData[isoCounter].Triangles = Triangles;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		//TEXT("Number of Triangles: ") + FString::FromInt(numTriangles));

	vtkDataArray* vtkNormalArray;
	vtkNormalArray = polyData->GetPointData()->GetNormals();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	//	TEXT("Number of Tuples: ") + FString::FromInt(vtkNormalArray->GetNumberOfTuples()));
	
	double testDouble[3];
	for (int i = 0; i < vtkNormalArray->GetNumberOfTuples(); i++) {
		vtkNormalArray->GetTuple(i, testDouble);
		normals.Add(FVector(testDouble[0], testDouble[1], testDouble[2]));
	}
	meshData[isoCounter].normals = normals;
}

void AMarchingCubes::DrawMesh() {
	if (prevElement != currElement) {
		prevElement = currElement;
		//int i = (isoValue * ((isoValueMax - isoValueMin) / numElements)) / step;
		int i = currElement;
		
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("iso value: ") + FString::FromInt(i) + TEXT(" struct iso value: ") + FString::SanitizeFloat(meshData[i].isoValue));
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("Num Vertices: ") + FString::SanitizeFloat(meshData[i].vertices.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("Num Triangles: ") + FString::SanitizeFloat(meshData[i].Triangles.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("Num normals: ") + FString::SanitizeFloat(meshData[i].normals.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			TEXT("Num vertex colors: ") + FString::SanitizeFloat(meshData[i].vertexColors.Num()));
			*/
		// draws the vertices and triangles in Unreal
		// most of the fields are unused for the purpose of this project, so just create empty arrays
		mesh->CreateMeshSection_LinearColor(0, meshData[i].vertices, meshData[i].Triangles, meshData[i].normals,
			TArray<FVector2D>(), meshData[i].vertexColors, TArray<FProcMeshTangent>(), true);

		// Enable collision data
		mesh->ContainsPhysicsTriMeshData(true);
	}
}

void AMarchingCubes::IncrementIsoValue(float newIsoValue)
{
	isoValue += newIsoValue * step;
}

void AMarchingCubes::IncrementElement(float increment)
{
	currElement += increment;
	if (currElement < 0) {
		currElement = 0;
	}
	else if (currElement > numElements) {
		currElement = numElements;
	}
}


void AMarchingCubes::IncreaseSpeed()
{
	step = 10.0;
}

void AMarchingCubes::DecreaseSpeed()
{
	step = 1.0;
}


void AMarchingCubes::MarchingCubes()
{	
	if (isoSet && !fileName.IsEmpty()) {
		for (isoCounter = 0; isoCounter < numElements + 1; isoCounter++) {
			meshData[isoCounter].isoValue = virtualIsoValue;
			meshData[isoCounter].element = isoCounter;

			//mesh->ClearAllMeshSections();
			polyData = vtkSmartPointer<vtkPolyData>::New();

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				TEXT("iso value: ") + FString::SanitizeFloat(meshData[isoCounter].isoValue));

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

	/*
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		TEXT("Min Iso Value: ") + FString::SanitizeFloat(isoValueMin));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		TEXT("Max Iso Value: ") + FString::SanitizeFloat(isoValueMax));
	*/
	numElements = (isoValueMax - isoValueMin) / step;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		//TEXT("Num Elements: ") + FString::SanitizeFloat(numElements));
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
