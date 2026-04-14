#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "C_GameMode.generated.h"

UCLASS()
class AURORAWORLDONLINE_API AC_GameMode : public AGameMode
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	AC_GameMode();
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
