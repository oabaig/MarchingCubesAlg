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

	vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> volumeOfInterest = vtkSmartPointer<vtkExtractVOI>::New();
	volumeOfInterest->SetInputConnection(xyPlaneColors->GetOutputPort());

	volumeOfInterest->SetVOI(0, 127, 0, 31, 0, 0);
	volumeOfInterest->Update();

	const char* oFname = TCHAR_TO_UTF8(
		*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("VTKSlices"), TEXT("myslice.jpg"))
	);

	jpegWriter->SetInputConnection(volumeOfInterest->GetOutputPort());
	jpegWriter->SetFileName(oFname);
	jpegWriter->Write();
}