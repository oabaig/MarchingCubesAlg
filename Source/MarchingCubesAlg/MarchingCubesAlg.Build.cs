// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class MarchingCubesAlg : ModuleRules
{
	public MarchingCubesAlg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd", "ClassViewer",
        "Raymarcher"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicDefinitions.Add("WITH_MYTHIRDPARTYLIBRARY=1");

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkChartsCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonColor-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonComputationalGeometry-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonDataModel-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonExecutionModel-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonMath-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonMisc-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonSystem-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkCommonTransforms-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkDICOMParser-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkDomainsChemistry-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkDomainsChemistryOpenGL2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkdoubleconversion-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkexodusII-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkexpat-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersAMR-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersExtraction-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersFlowPaths-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersGeneral-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersGeneric-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersGeometry-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersHybrid-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersHyperTree-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersImaging-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersModeling-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersParallel-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersParallelImaging-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersPoints-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersProgrammable-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersSelection-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersSMP-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersSources-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersStatistics-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersTexture-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersTopology-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkFiltersVerdict-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkfreetype-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkGeovisCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkgl2ps-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkglew-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkhdf5-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkhdf5_hl-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingColor-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingFourier-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingGeneral-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingHybrid-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingMath-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingMorphological-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingSources-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingStatistics-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkImagingStencil-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkInfovisCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkInfovisLayout-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkInteractionImage-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkInteractionStyle-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkInteractionWidgets-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOAMR-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOAsynchronous-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOCityGML-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOEnSight-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOExodus-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOExport-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOExportGL2PS-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOExportPDF-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOGeometry-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOImage-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOImport-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOInfovis-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOLegacy-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOLSDyna-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOMINC-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOMotionFX-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOMovie-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIONetCDF-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOOggTheora-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOParallel-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOParallelXML-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOPLY-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOSegY-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOSQL-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOTecplotTable-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOVeraOut-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOVideo-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOXML-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkIOXMLParser-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkjpeg-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkjsoncpp-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtklibharu-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtklibproj-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtklibxml2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkloguru-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtklz4-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtklzma-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkmetaio-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtknetcdf-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkogg-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkParallelCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkParallelDIY-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkpng-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkpugixml-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingAnnotation-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingContext2D-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingContextOpenGL2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingFreeType-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingGL2PSOpenGL2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingImage-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingLabel-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingLOD-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingOpenGL2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingSceneGraph-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingUI-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingVolume-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingVolumeOpenGL2-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkRenderingVtkJS-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtksqlite-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtksys-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkTestingRendering-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtktheora-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtktiff-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkverdict-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkViewsContext2D-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkViewsCore-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkViewsInfovis-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkWrappingTools-9.0.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "vtkzlib-9.0.lib"));



		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
