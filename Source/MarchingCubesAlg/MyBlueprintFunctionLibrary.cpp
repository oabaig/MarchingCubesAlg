#include "MyBlueprintFunctionLibrary.h"
#include "Runtime\Core\Public\Misc\FileHelper.h"
#include "Runtime\Core\Public\Misc\Paths.h"
#include "Developer\DesktopPlatform\Public\IDesktopPlatform.h"
#include "Developer\DesktopPlatform\Public\DesktopPlatformModule.h"

FString UMyBlueprintFunctionLibrary::OpenWindowsFiles() {
	TArray<FString> OpenFileNames;//The absolute path of the obtained file
	FString ExtensionStr = TEXT("*.vtk*");//File type

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	DesktopPlatform->OpenFileDialog(nullptr, TEXT("ywy file manager"), FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), TEXT(""), *ExtensionStr, EFileDialogFlags::None, OpenFileNames);
	return OpenFileNames[0];
}