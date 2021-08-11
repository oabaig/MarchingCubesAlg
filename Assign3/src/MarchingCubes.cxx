#include "vtkDataSetReader.h"
#include "vtkSmartPointer.h"

int main(){
	vtkSmartPointer<vtkDataSetReader> reader;
	reader->SetFileName("../../Content/Data/test.vtk");

	
	return 0;
}