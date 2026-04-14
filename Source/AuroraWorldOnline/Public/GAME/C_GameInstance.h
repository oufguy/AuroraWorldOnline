#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "CAMERA_LIBRARY.h"

#include "C_GameInstance.generated.h"

UCLASS()
class AURORAWORLDONLINE_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	UC_GameInstance();
	
	virtual void Init() override;
	virtual void Shutdown() override;
	
	// ========================================================================
	// CAMERA SYSTEM
	// ========================================================================
public:
	UPROPERTY()
	FCamera_Settings Camera_Settings;
};
