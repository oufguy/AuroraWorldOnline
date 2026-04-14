#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Actor.h"

#include "MOVEMENT_LIBRARY.h"

class AC_PlayerCharacter;

#include "C_PlayerState.generated.h"


UCLASS()
class AURORAWORLDONLINE_API AC_PlayerState : public APlayerState
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	AC_PlayerState();
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//==============================================================
	// PLAYER SYSTEM
	//==============================================================
public:
	
	UPROPERTY(Replicated)
	int32 UserID = 0;
	
	UPROPERTY()
	TObjectPtr<AC_PlayerCharacter> PlayerCharacter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | PLAYER")
	TSubclassOf<AC_PlayerCharacter> PlayerCharacter_Class = nullptr;
	
	void Spawn_PlayerCharacter();
};
