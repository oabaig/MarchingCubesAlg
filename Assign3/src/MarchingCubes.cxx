#include "vtkDataSetReader.h"
#include "vtkSmartPointer.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"
#include "vtkLookupTable.h"
#include "vtkDataSet.h"
#include "vtkStructuredPoints.h"
#include "vtkImageData.h"
#include "vtkImageMapper3D.h"
#include "vtkJPEGWriter.h"
#include "vtkExtractVOI.h"
#include "vtkImageAppend.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"

#include <sstream>
#include <iostream>

/* NOTES
	if you get an error that says '(method) is not a member of vtkSmartPointer<(class)>', make sure you replace . to ->
*/

int main(){
	vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();
	reader->SetFileName("C:/Dev/UnrealProjects/MarchingCubesAlg/Assign3/src/bernard3D_Q.vtk");
	reader->Update();

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkColorTransferFunction> volumeColor = vtkSmartPointer<vtkColorTransferFunction>::New();
	volumeColor->AddRGBPoint(-10, 1.0, 1.0, 0.9);
	volumeColor->AddRGBPoint(-5, 1.0, 1.0, 0.3);
	volumeColor->AddRGBPoint(5, 1.0, 0.5, 0.3);
	volumeColor->AddRGBPoint(10, 1.0, 1.0, 0.9);

	vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	volumeScalarOpacity->AddPoint(-10, 0.15);
	volumeScalarOpacity->AddPoint(-5, 0.15);
	volumeScalarOpacity->AddPoint(5, 0.85);
	volumeScalarOpacity->AddPoint(10, 1.0);

	vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	volumeGradientOpacity->AddPoint(0, 0.0);
	volumeGradientOpacity->AddPoint(90, 0.5);	
	volumeGradientOpacity->AddPoint(100, 1.0);	

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(volumeColor);
	volumeProperty->SetScalarOpacity(volumeScalarOpacity);
	volumeProperty->SetGradientOpacity(volumeGradientOpacity);
	volumeProperty->SetInterpolationTypeToLinear();

	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.2);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.1, 0.4, 0.2);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetWindowName("Volume Renderer");
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// xy plane cutter
	int current_zID = 0;

	vtkSmartPointer<vtkLookupTable> bwLut = vtkSmartPointer<vtkLookupTable>::New();
	bwLut->SetTableRange(0, 2);
	bwLut->SetSaturationRange(0, 0);
	bwLut->SetHueRange(0, 0);
	bwLut->SetValueRange(0, 1);
	bwLut->Build();

	vtkSmartPointer<vtkImageMapToColors> xy_plane_Colors = vtkSmartPointer<vtkImageMapToColors>::New();
	xy_plane_Colors->SetInputConnection(reader->GetOutputPort());
	xy_plane_Colors->SetLookupTable(bwLut);
	xy_plane_Colors->Update();
	
	int* dim = reader->GetStructuredPointsOutput()->GetDimensions();

	//std::cout << dim[0] << " " << dim[1] << std::endl;
	
	vtkSmartPointer<vtkImageActor> xy_plane = vtkSmartPointer<vtkImageActor>::New();
	xy_plane->GetMapper()->SetInputConnection(xy_plane_Colors->GetOutputPort());
	vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();

	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(xy_plane->GetInput());

	for (current_zID; current_zID < 64; current_zID++) {
		xy_plane->SetDisplayExtent(0, 128, 0, 128, current_zID, current_zID);
		xy_plane->Update();
		
		std::stringstream ss;

		ss << current_zID;

		std::string prefixName = "test";
		std::string suffixName = ".jpg";
		std::string stringInt;

		ss >> stringInt;

		voi->SetVOI(0, 127, 0, 31, current_zID, current_zID);
		voi->Update();

		std::string fname = prefixName + stringInt + suffixName;

		//images->AddInputData(voi->GetOutput());

		//vtkSmartPointer<vtkImageData> outputImage = vtkSmartPointer<vtkImageData>::New();

		//outputImage->ShallowCopy(voi->GetOutput());
		//images->AddInputData(outputImage);
		//images->Update();

		jpegWriter->SetInputData(voi->GetOutput());
		jpegWriter->SetFileName(fname.c_str());
		jpegWriter->Write();
	}
	/*
	jpegWriter->SetFileName("appendedImages.jpg");
	jpegWriter->SetInputData(images->GetOutput());
	jpegWriter->Write();*/
	

	vtkSmartPointer<vtkImageAppend> horizontalImages = vtkSmartPointer<vtkImageAppend>::New();

	vtkSmartPointer<vtkImageAppend> verticalImages = vtkSmartPointer<vtkImageAppend>::New();
	verticalImages->SetAppendAxis(1);
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();

	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0 && i > 0) {
			horizontalImages->AddInputData(verticalImages->GetOutput());
			horizontalImages->Update();

			verticalImages->RemoveAllInputs();
			verticalImages->SetAppendAxis(1);
		}
		std::stringstream ss;

		ss << i;

		std::string prefixName = "test";
		std::string suffixName = ".jpg";
		std::string stringInt;

		ss >> stringInt;

		std::string fname = prefixName + stringInt + suffixName;

		vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();

		jpegReader->SetFileName(fname.c_str());
		jpegReader->Update();
		imageData->ShallowCopy(jpegReader->GetOutput());
		verticalImages->AddInputData(imageData);
		verticalImages->Update();
		
	}

	horizontalImages->AddInputData(verticalImages->GetOutput());
	horizontalImages->Update();

	std::cout << verticalImages->GetNumberOfInputs() << std::endl;

	jpegWriter->SetInputData(horizontalImages->GetOutput());
	jpegWriter->SetFileName("appendedImages.jpg");
	jpegWriter->Write();
	
	renderer->AddActor(xy_plane);
	// end xy plane cutter

	renderer->AddViewProp(volume);
	renderer->ResetCamera();

	renderWindow->Render();
	renderWindowInteractor->Start();

	
	return 0;
}