// Fill out your copyright notice in the Description page of Project Settings.

#include "VTKFileImporter_funclib.h"

UVTKFileImporter_funclib::UVTKFileImporter_funclib() {
	_reader = vtkSmartPointer<vtkDataSetReader>::New();
}

void UVTKFileImporter_funclib::SetFileName(FString fname) {
	_reader->SetFileName(TCHAR_TO_ANSI(*fname));
	_reader->Update();
}

void UVTKFileImporter_funclib::SliceImages() {
	/*
	vtkSmartPointer<vtkLookupTable> bwLut = vtkSmartPointer<vtkLookupTable>::New();
	bwLut->SetTableRange(0, 2);
	bwLut->SetSaturationRange(0, 0);
	bwLut->SetHueRange(0, 0);
	bwLut->SetValueRange(0, 1);
	bwLut->Build();

	vtkSmartPointer<vtkImageMapToColors> xyPlaneColors = vtkSmartPointer<vtkImageMapToColors>::New();
	xyPlaneColors->SetInputConnection(_reader->GetOutputPort());
	xyPlaneColors->SetLookupTable(bwLut);
	xyPlaneColors->Update();

	vtkSmartPointer<vtkImageActor> xyPlane = vtkSmartPointer<vtkImageActor>::New();
	xyPlane->GetMapper()->SetInputConnection(xyPlaneColors->GetOutputPort());

	vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> volumeOfInterest = vtkSmartPointer<vtkExtractVOI>::New();
	volumeOfInterest->SetInputData(xyPlane)*/
}