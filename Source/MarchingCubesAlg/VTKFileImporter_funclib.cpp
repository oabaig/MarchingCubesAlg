// Fill out your copyright notice in the Description page of Project Settings.

#include "VTKFileImporter_funclib.h"

void UVTKFileImporter_funclib::SliceImages(FString filename) {
	vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();

	reader->SetFileName(TCHAR_TO_ANSI(*filename));
	reader->Update();

	// fix?
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
	volumeOfInterest->SetInputData(xyPlaneColors->GetOutput());

	// fix
	for (int i = 0; i < 64; i++) {
		std::stringstream ss;

		ss << i;
		
		// fix
		std::string prefixName = "slice";
		std::string suffixName = ".jpg";
		std::string stringInt;

		ss >> stringInt;

		// fix
		volumeOfInterest->SetVOI(0, 127, 0, 31, i, i);
		volumeOfInterest->Update();

		std::string fname = prefixName + stringInt + suffixName;

		jpegWriter->SetInputData(volumeOfInterest->GetOutput());
		jpegWriter->SetFileName(fname.c_str());
		//jpegWriter->Write();
	}
}