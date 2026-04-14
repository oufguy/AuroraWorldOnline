#include "PLAYER/C_PlayerState.h"
#include "SHARED_FUNC_LIB.h"

#include "Net/UnrealNetwork.h"

#include "PLAYER/C_PlayerCharacter.h"
#include "PLAYER/C_PlayerController.h"

#include "MAP_PARAMS.h"
#include "PLAYER/C_PlayerController.h"

// ========================================================================
// SHARED
// ========================================================================

AC_PlayerState::AC_PlayerState()
{
}

void AC_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Check if World is Avaible
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		// 1. Only the Server has the power to spawn replicated actors
		if (HasAuthority())
		{
			UserID = FMath::RandRange(1, 2000000);
		
			RLOG_W("Spawning for Client: UserID %d", UserID)
			Spawn_PlayerCharacter();
		}
	}
	else ratto_return("World is NOT Ready");
}

void AC_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ThisClass, UserID, COND_OwnerOnly);
}

void AC_PlayerState::Spawn_PlayerCharacter()
{
	// Prevent Null & Crash
	if (!IsValid(PlayerCharacter_Class)) ratto_return("PLEASE SET PlayerCharacter_Class");

	// Set Actor SpawnParams
	FActorSpawnParameters SpawnParams;
	// optional : SpawnParams.Owner = NewPlayer;
	// optional : SpawnParams.Instigator = NewPlayer->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// optional : SpawnParams.Name = FName(*FString::Printf(TEXT("PlayerCharacter_%d"), UserID));
	
	FVector SpawnLocation = FVector(0.f, 0.f, 100.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	// Spawn Pawn & Set Pawn
	PlayerCharacter = GetWorld()->SpawnActor<AC_PlayerCharacter>(PlayerCharacter_Class, SpawnLocation, SpawnRotation, SpawnParams);
	PlayerCharacter->UserID = UserID;
}
