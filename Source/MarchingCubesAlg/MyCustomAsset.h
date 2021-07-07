#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyCustomAsset.generated.h"

/**
 *
 */
UCLASS()
class MARCHINGCUBESALG_API UMyCustomAsset : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = MyCustomAsset)
		FString Description;

	UPROPERTY(VisibleAnywhere, Category = MyCustomAsset)
		bool bIsActive;
};