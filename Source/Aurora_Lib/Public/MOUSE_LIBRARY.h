#pragma once

#include "CoreMinimal.h"
#include "MOUSE_LIBRARY.generated.h"

// ========================================================================
// MOUSE SYSTEM
// ========================================================================

enum class EMouse_InputMode : uint8
{
	GameOnly     UMETA(DisplayName = "Game Only"),
	GameAndUI    UMETA(DisplayName = "Game And UI"),
};

// ========================================================================
// HIGHLIGHT SYSTEM
// ========================================================================

USTRUCT()
struct FHighlight_SendRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> Highlight_CollisionChannel = ECC_Visibility;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> MaterialToUse = nullptr;
	
	// ========================================================================
	// OPTIONS
	// ========================================================================
	
	// Setting to Prevent Action
	bool bCan_Highlight = false;
};
