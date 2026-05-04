#include "PLAYER/C_PlayerState.h"
#include "SHARED_FUNC_LIB.h"

#include "Net/UnrealNetwork.h"

#include "GAME/C_GameMode.h"

#include "PLAYER/C_PlayerSoul.h"
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
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
	
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
		
		// Attach the PlayerSoul
		Attach_PlayerSoul();
		RLOG_W("Player Character has been Spawned")
		
		Ref_GameMode->List_PlayerCharacters_Add(UserID, Ref_PlayerCharacter);
	}
	else
	{
		ratto_return("Player Character has been Found, Skipping Spawn");
	}
	
}

void AC_PlayerState::Attach_PlayerSoul()
{
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not Have Authority");
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
	
	// Check Pawn
	if (!GetPawn()) ratto_return("Pawn is NOT READY");
	if (SAFE_CAST(GetPawn(), AC_PlayerSoul::StaticClass()))
	{
		Ref_PlayerSoul = Cast<AC_PlayerSoul>(GetPawn());
		RLOG_W("Player Soul Successfully CAST")
	}
	else ratto_return("PLAYER SOULD FAILED TO CAST");
	
	// Check PlayerSoul
	if (!IsValid(Ref_PlayerSoul)) ratto_return("Ref_PlayerSoul is NOT READY");
	
	RLOG_W("PlayerSoul's Current Position : %f, %f, %f", Ref_PlayerSoul->GetActorLocation().X, Ref_PlayerSoul->GetActorLocation().Y, Ref_PlayerSoul->GetActorLocation().Z);
	
	// Attach PlayerSoul into PlayerCharacter CameraSocket
	Ref_PlayerSoul->AttachToComponent(Ref_PlayerCharacter->CameraSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	RLOG_W("PlayerSoul's NEW Position : %f, %f, %f", Ref_PlayerSoul->GetActorLocation().X, Ref_PlayerSoul->GetActorLocation().Y, Ref_PlayerSoul->GetActorLocation().Z);
	
}
