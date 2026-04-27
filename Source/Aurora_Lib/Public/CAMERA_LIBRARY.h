#pragma once

#include "CoreMinimal.h"
#include "CAMERA_LIBRARY.generated.h"

// ========================================================================
// CAMERA SYSTEM
// ========================================================================

USTRUCT(BlueprintType)
struct FCamera_Values
{
	GENERATED_BODY()
	
	// ========================================================================
	// SHARED
	// ========================================================================
	
	bool Camera_Attached = false;
	
	// ========================================================================
	// ZOOM
	// ========================================================================
	
	float Current_Zoom = 300.0f;
	float Min_Zoom = 100.0f;
	float Max_Zoom = 1500.0f;
	float Target_Zoom = 500.0f;
	
	// ========================================================================
	// ROTATION
	// ========================================================================

	float Current_Horizontal = 0.0f;
	float Target_Horizontal = 0.0f;
	
	float Current_Vertical = 0.0f;
	float Min_Vertical = -80.0f;
	float Max_Vertical = 80.0f;
	float Target_Vertical = 0.0f;
	
	// ========================================================================
	// OPTIONS
	// ========================================================================
	
	// Setting to Prevent Action
	bool bCan_Zoom = false;
	bool bCan_Rotate = false;
};

USTRUCT(BlueprintType)
struct FCamera_Settings
{
	GENERATED_BODY()
	
	// ========================================================================
	// SHARED
	// ========================================================================
	
	UPROPERTY(EditAnywhere)
	float Fade_CameraBlock_Dist = 300.0f;
	
	UPROPERTY(EditAnywhere)
	float Zoom_Sensitivity = 50.0f;
	UPROPERTY(EditAnywhere)
	float Zoom_Speed = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float Horizontal_Sensitivity = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float Vertical_Sensitivity = 0.5f;
	
	// ========================================================================
	// OPTIONS
	// ========================================================================
	
	UPROPERTY(EditAnywhere)
	bool Zoom_Inverted = false;
	UPROPERTY(EditAnywhere)
	bool Horizontal_Inverted = true;
	UPROPERTY(EditAnywhere)
	bool Vertical_Inverted = false;
};

// ========================================================================
// FADE SYSTEM
// ========================================================================

USTRUCT(BlueprintType)
struct FFade_SendRequest
{
	GENERATED_BODY()
	
	// Trace Channel for HitResult
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> Fade_TraceChannel = TraceTypeQuery1;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> Fade_CollisionChannel = ECC_Visibility;
	
	// Chosen Component to Add
	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> TargetComponent = nullptr;

	// Chosen Material to Use
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> FadeMaterial = nullptr;
	// Parameter Name to Get Material Params
	UPROPERTY(EditAnywhere)
	FName FadeParameterName = "FadeParam";
	
	// ========================================================================
	// OPTIONS
	// ========================================================================
	
	// Setting to Prevent Action
	bool bCan_Fade = false;
	
	// Animation time to Slow/Fasten it
	UPROPERTY(EditAnywhere)
	float AnimationTime = 0.5f;
};