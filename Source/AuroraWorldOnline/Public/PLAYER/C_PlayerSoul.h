#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "C_PlayerSoul.generated.h"

UCLASS()
class AURORAWORLDONLINE_API AC_PlayerSoul : public APawn
{
	GENERATED_BODY()
	
	// ========================================================================
	// SHARED
	// ========================================================================
public:	
	AC_PlayerSoul();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
