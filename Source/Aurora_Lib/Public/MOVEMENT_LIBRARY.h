#pragma once

#include "CoreMinimal.h"
#include "MOVEMENT_LIBRARY.generated.h"

//==============================================================
// MOVEMENT SYSTEM
//==============================================================

UENUM()
enum class EMovementState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	//==================================================//
	Forward         UMETA(DisplayName = "Forward"),
	Backward        UMETA(DisplayName = "Backward"),
	Left            UMETA(DisplayName = "Left"),
	Right           UMETA(DisplayName = "Right"),
	//==================================================//
	Forward_Left    UMETA(DisplayName = "Forward Left"),
	Forward_Right   UMETA(DisplayName = "Forward Right"),
	Backward_Left   UMETA(DisplayName = "Backward Left"),
	Backward_Right  UMETA(DisplayName = "Backward Right")
};

USTRUCT()
struct FMovementStruct
{
	GENERATED_BODY()
	
	EMovementState MovementState = EMovementState::Idle;
	bool bIsJumping = false;
};
