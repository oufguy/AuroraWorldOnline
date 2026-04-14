#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MAP_PARAMS.generated.h"

USTRUCT(BlueprintType)
struct FMap_SpawnParams
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation = FRotator::ZeroRotator;
};

UENUM(BlueprintType)
enum class ESpawnMap : uint8
{
    NONE         UMETA(DisplayName = "None"),
    DEV_MAP      UMETA(DisplayName = "Development Map"),
};

UCLASS()
class AURORA_LIB_API UMAP_PARAMS : public UDataAsset
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
	
    static bool IsSpawnAble(FString MapName);
	
	static FMap_SpawnParams Get_MapSpawnPoints(ESpawnMap ChosenMap, int8 SpawnPoint_No);
};
