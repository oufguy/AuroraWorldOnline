#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ACTOR_COMPONENTS/C_ClientSystems_AC.h"

#include "C_PlayerController.generated.h"

UCLASS()
class AURORAWORLDONLINE_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	// ========================================================================
	// SHARED
	// ========================================================================
public:
	AC_PlayerController();

	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | ClientSystems")
	TSubclassOf<UC_ClientSystems_AC> Ref_ClientSystems_Class = nullptr;
	UPROPERTY()
	TObjectPtr<UC_ClientSystems_AC> Ref_ClientSystems = nullptr;
	
	void Create_ClientSystems_ActorComponent();
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	
	// ========================================================================
	// MOVEMENT SYSTEM
	// ========================================================================
	
	UFUNCTION(Server, Reliable)
	void Receive_MovementDirections(const bool bIsJumping, const uint8 MovementState);
	void Receive_MovementDirections_Implementation(const bool bIsJumping, const uint8 MovementState);
};
