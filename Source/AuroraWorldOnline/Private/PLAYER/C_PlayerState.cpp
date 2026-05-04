#include "PLAYER/C_PlayerState.h"
#include "SHARED_FUNC_LIB.h"

#include "Net/UnrealNetwork.h"

#include "GAME/C_GameMode.h"

#include "PLAYER/C_PlayerCharacter.h"
#include "PLAYER/C_PlayerController.h"

#include "MAP_PARAMS.h"

// ========================================================================
// SHARED
// ========================================================================

AC_PlayerState::AC_PlayerState()
{
}

void AC_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not Have Authority");
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
	
	// TEST PROPERTY : SPAWN CHARACTER
	Spawn_PlayerCharacter();	
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

//==============================================================
// PLAYER SYSTEM
//==============================================================

void AC_PlayerState::Spawn_PlayerCharacter()
{
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not Have Authority");
	
	// Prevent Null & Crash
	if (!IsValid(Ref_PlayerCharacter_Class)) ratto_return("PLEASE SET PlayerCharacter_Class");
	
	// Check GameMade Reference
	if (!IsValid(Ref_GameMode)) ratto_return("Ref_GameMode is Invalid");
	
	// Find if PlayerCharacter already Exists
	Ref_PlayerCharacter = Ref_GameMode->List_PlayerCharacters_Find(UserID);
	
	if (Ref_PlayerCharacter == nullptr)
	{
		RLOG_W("Player Character has not been Found, Spawning...")
		
		// Set Actor SpawnParams
		FActorSpawnParameters SpawnParams;
		// optional : SpawnParams.Owner = NewPlayer;
		// optional : SpawnParams.Instigator = NewPlayer->GetPawn();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// optional : SpawnParams.Name = FName(*FString::Printf(TEXT("PlayerCharacter_%d"), UserID));
	
		FVector SpawnLocation = FVector(0.f, 0.f, 100.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
	
		// Spawn Pawn & Set Pawn
		Ref_PlayerCharacter = GetWorld()->SpawnActor<AC_PlayerCharacter>(Ref_PlayerCharacter_Class, SpawnLocation, SpawnRotation, SpawnParams);
		Ref_PlayerCharacter->UserID = UserID;
	
		Ref_GameMode->List_PlayerCharacters_Add(UserID, Ref_PlayerCharacter);
	}
	else
	{
		ratto_return("Player Character has been Found, Skipping Spawn");
	}
	
}
