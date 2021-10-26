// Fill out your copyright notice in the Description page of Project Settings.

#include "VTKFileImporter_funclib.h"

void UVTKFileImporter_funclib::SliceImages(FString fname) {
	vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();
	reader->SetFileName(TCHAR_TO_ANSI(*fname));
	reader->Update();


	vtkSmartPointer<vtkLookupTable> bwLut = vtkSmartPointer<vtkLookupTable>::New();
	bwLut->SetTableRange(0, 2);
	bwLut->SetSaturationRange(0, 0);
	bwLut->SetHueRange(0, 0);
	bwLut->SetValueRange(0, 1);
	bwLut->Build();

	vtkSmartPointer<vtkImageMapToColors> xyPlaneColors = vtkSmartPointer<vtkImageMapToColors>::New();
	xyPlaneColors->SetInputConnection(reader->GetOutputPort());
	xyPlaneColors->SetLookupTable(bwLut);
	xyPlaneColors->Update();
	/*
	vtkSmartPointer<vtkPNGWriter> imgWriter = vtkSmartPointer<vtkPNGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> volumeOfInterest = vtkSmartPointer<vtkExtractVOI>::New();
	volumeOfInterest->SetInputConnection(xyPlaneColors->GetOutputPort());

	vtkSmartPointer<vtkImageAppend> rowImages = vtkSmartPointer<vtkImageAppend>::New();
	rowImages->SetAppendAxis(1);
	vtkSmartPointer<vtkImageAppend> columnImages = vtkSmartPointer<vtkImageAppend>::New();
	columnImages->SetAppendAxis(2);
	int numImageData = 0;

	vtkSmartPointer<vtkImageData> verticalImageData[8];
	int numVerticalImageData = 0;
	for (int i = 0; i < 64; i++) {
		/*
		if (i % 8 == 0 && i > 0) {
			//columnImages->Update();	
			numImageData = 0;
			
			verticalImageData[numVerticalImageData] = vtkSmartPointer<vtkImageData>::New();
			verticalImageData[numVerticalImageData]->DeepCopy(columnImages->GetOutput());

			rowImages->AddInputData(verticalImageData[numVerticalImageData++]);

			columnImages->RemoveAllInputs();

		}
		volumeOfInterest->SetVOI(0, 31, 0, 31, i, i);
		volumeOfInterest->Update();
		/*
		std::stringstream ss;

		ss << i;

		std::string stringInt;
		ss >> stringInt;

		FString stringIntFName = stringInt.c_str();
		
		FString oSliceName = "MySlice" + stringIntFName + ".png";
		FString oDirectoryName = "VTKSlices/" + oSliceName;

		const char* oFname = TCHAR_TO_UTF8(
			*FPaths::Combine(FPaths::ProjectContentDir(), oDirectoryName)
		);
		

		/*
		imageData[numImageData] = vtkSmartPointer<vtkImageData>::New();
		imageData[numImageData]->DeepCopy(volumeOfInterest->GetOutput());

		columnImages->AddInputData(imageData[numImageData++]);

		vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
		imageData->ShallowCopy(volumeOfInterest->GetOutput());
		columnImages->AddInputData(imageData);
	}
	columnImages->Update();
	//horizontalImages->AddInputData(verticalImages->GetOutput());
	//rowImages->Update();
	*/


	vtkSmartPointer<vtkPNGWriter> imgWriter = vtkSmartPointer<vtkPNGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> volumeOfInterest = vtkSmartPointer<vtkExtractVOI>::New();
	volumeOfInterest->SetInputData(xyPlaneColors->GetOutput());

	for (int i = 0; i < 64; i++) {
		volumeOfInterest->SetVOI(0, 31, 0, 31, i, i);
		volumeOfInterest->Update();

		std::stringstream ss;

		ss << i;

		std::string stringInt;
		ss >> stringInt;

		FString stringIntFName = stringInt.c_str();

		FString oSliceName = "MySlice" + stringIntFName + ".png";
		FString oDirectoryName = "VTKSlices/" + oSliceName;

		const char* oFname = TCHAR_TO_UTF8(
			*FPaths::Combine(FPaths::ProjectContentDir(), oDirectoryName)
		);

		imgWriter->SetInputData(volumeOfInterest->GetOutput());
		imgWriter->SetFileName(oFname);
		imgWriter->Write();
	}

	vtkSmartPointer<vtkImageAppend> horizontalImages = vtkSmartPointer<vtkImageAppend>::New();
	horizontalImages->SetAppendAxis(1);
	vtkSmartPointer<vtkImageAppend> verticalImages = vtkSmartPointer<vtkImageAppend>::New();
	//verticalImages->SetAppendAxis(1);
	vtkSmartPointer<vtkPNGReader> imgReader = vtkSmartPointer<vtkPNGReader>::New();

	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0 && i > 0) {
			verticalImages->Update();

			vtkSmartPointer<vtkImageData> imgHorizontalData = vtkSmartPointer<vtkImageData>::New();

			imgHorizontalData->ShallowCopy(verticalImages->GetOutput());

			horizontalImages->AddInputData(imgHorizontalData);

			verticalImages->RemoveAllInputs();
			//verticalImages->SetAppendAxis(1);
		}

		std::stringstream ss;

		ss << i;

		std::string stringInt;
		ss >> stringInt;

		FString stringIntFName = stringInt.c_str();

		FString oSliceName = "MySlice" + stringIntFName + ".png";
		FString oDirectoryName = "VTKSlices/" + oSliceName;

		const char* oFname = TCHAR_TO_UTF8(
			*FPaths::Combine(FPaths::ProjectContentDir(), oDirectoryName)
		);

		imgReader->SetFileName(oFname);
		imgReader->Update();

		vtkSmartPointer<vtkImageData> imgData = vtkSmartPointer<vtkImageData>::New();

		imgData->ShallowCopy(imgReader->GetOutput());

		verticalImages->AddInputData(imgData);
	}
	verticalImages->Update();
	vtkSmartPointer<vtkImageData> imgHorizontalData = vtkSmartPointer<vtkImageData>::New();
	imgHorizontalData->ShallowCopy(verticalImages->GetOutput());
	horizontalImages->AddInputData(imgHorizontalData);
	horizontalImages->Update();

	vtkSmartPointer<vtkImageFlip> flipFilter = vtkSmartPointer<vtkImageFlip>::New();
	flipFilter->SetFilteredAxes(1);
	flipFilter->SetInputConnection(horizontalImages->GetOutputPort());
	flipFilter->Update();

	FString oSliceName = "appendedImages.png";
	FString oDirectoryName = "VTKSlices/" + oSliceName;

	const char* oFname = TCHAR_TO_UTF8(
		*FPaths::Combine(FPaths::ProjectContentDir(), oDirectoryName)
	);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		oFname);

	imgWriter->SetInputData(flipFilter->GetOutput());
	imgWriter->SetFileName(oFname);
	imgWriter->Write();
}

void UVTKFileImporter_funclib::StichImages() {
	vtkSmartPointer<vtkPNGWriter> imgWriter = vtkSmartPointer<vtkPNGWriter>::New();

	vtkSmartPointer<vtkImageAppend> horizontalImages = vtkSmartPointer<vtkImageAppend>::New();

	vtkSmartPointer<vtkImageAppend> verticalImages = vtkSmartPointer<vtkImageAppend>::New();
	verticalImages->SetAppendAxis(1);
	vtkSmartPointer<vtkPNGReader> imgReader = vtkSmartPointer<vtkPNGReader>::New();
	vtkSmartPointer<vtkImageData> imageData[64];

	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0 && i > 0) {
			verticalImages->Update();
			horizontalImages->AddInputData(verticalImages->GetOutput());

			verticalImages->RemoveAllInputs();
			verticalImages->SetAppendAxis(1);
		}
		std::stringstream ss;

		ss << i;

		std::string prefixName = "MySlice";
		std::string suffixName = ".png";
		std::string stringInt;

		ss >> stringInt;

		std::string fname = prefixName + stringInt + suffixName;
		imageData[i] = vtkSmartPointer<vtkImageData>::New();

		imgReader->SetFileName(fname.c_str());
		imgReader->Update();

		imageData[i]->DeepCopy(imgReader->GetOutput());
		verticalImages->AddInputData(imageData[i]);
	}

	horizontalImages->AddInputData(verticalImages->GetOutput());
	horizontalImages->Update();

	FString oSliceName = "appendedImages.png";
	FString oDirectoryName = "VTKSlices/" + oSliceName;

	const char* oFname = TCHAR_TO_UTF8(
		*FPaths::Combine(FPaths::ProjectContentDir(), oDirectoryName)
	);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		oFname);

	imgWriter->SetInputData(horizontalImages->GetOutput());
	imgWriter->SetFileName(oFname);
	imgWriter->Write();
}