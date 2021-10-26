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

	vtkSmartPointer<vtkPNGWriter> imgWriter = vtkSmartPointer<vtkPNGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> volumeOfInterest = vtkSmartPointer<vtkExtractVOI>::New();
	volumeOfInterest->SetInputConnection(xyPlaneColors->GetOutputPort());

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

		imgWriter->SetInputConnection(volumeOfInterest->GetOutputPort());
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
			vtkSmartPointer<vtkImageData> verticalImgData = vtkSmartPointer<vtkImageData>::New();

			verticalImgData->ShallowCopy(verticalImages->GetOutput());
			horizontalImages->AddInputData(verticalImgData);

			verticalImages->RemoveAllInputs();
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
	vtkSmartPointer<vtkImageData> verticalImgData = vtkSmartPointer<vtkImageData>::New();

	verticalImgData->ShallowCopy(verticalImages->GetOutput());
	horizontalImages->AddInputData(verticalImgData);
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

	imgWriter->SetInputData(flipFilter->GetOutput());
	imgWriter->SetFileName(oFname);
	imgWriter->Write();
}