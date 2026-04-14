#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "C_GameState.generated.h"


UCLASS()
class AURORAWORLDONLINE_API AC_GameState : public AGameState
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	AC_GameState();
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
