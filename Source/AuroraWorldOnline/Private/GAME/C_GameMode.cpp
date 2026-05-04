#include "GAME/C_GameMode.h"
#include "SHARED_FUNC_LIB.h"

#include "PLAYER/C_PlayerState.h"
#include "PLAYER/C_PlayerCharacter.h"

// ========================================================================
// SHARED
// ========================================================================

AC_GameMode::AC_GameMode()
{
}

void AC_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AC_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// Check if Player is Valid
	if (!NewPlayer) ratto_return("NewPlayer is Invalid");
	
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not have Authority");
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
	
	// Reference Object for Faster Access
	TObjectPtr<AC_PlayerState> Ref_PlayerState = nullptr;
			
	// Check if PlayerState of Player is Valid
	if (!IsValid(Ref_PlayerState = NewPlayer->GetPlayerState<AC_PlayerState>())) ratto_return("PlayerState is Invalid");
	// Set the GameMode Reference for the PlayerState
	Ref_PlayerState->Ref_GameMode = this;
				
	// TEST PROPERTY : RANDOMIZED USERID
	Ref_PlayerState->UserID = FMath::RandRange(1, 2000000);
	
}

// ========================================================================
// PLAYER SYSTEM
// ========================================================================

AC_PlayerCharacter* AC_GameMode::List_PlayerCharacters_Find(int32 UserID)
{
	// Check Authority
	if (!HasAuthority())
	{
		RLOG_E("Does not have Authority");
		return nullptr;
	}
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		RLOG_E("World is NOT Ready");
		return nullptr;
	}
		
	// Check if UserID and PlayerCharacter are Valid
	if (UserID <= 0)
	{
		RLOG_E("UserID is Invalid")
		return nullptr;
	}
			
	// Find from TMap
	if (List_PlayerCharacters.Contains(UserID))
	{
		// Return the PlayerCharacter
		return List_PlayerCharacters[UserID];
	}
	else
	{
		RLOG_E("PlayerCharacter not Found in TMap")
		return nullptr;
	}
}

void AC_GameMode::List_PlayerCharacters_Add(int32 UserID, AC_PlayerCharacter *New_PlayerCharacter)
{
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not have Authority");
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
		
	// Check if UserID and PlayerCharacter are Valid
	if (UserID <= 0) ratto_return("UserID is Invalid");
	if (!IsValid(New_PlayerCharacter)) ratto_return("PlayerCharacter is Invalid");
			
	// Add to TMap
	List_PlayerCharacters.Add(UserID, New_PlayerCharacter);
}

void AC_GameMode::List_PlayerCharacters_Remove(int32 UserID)
{
	// Check Authority
	if (!HasAuthority()) ratto_return("Does not have Authority");
	// Check if World is Available
	if (!GetWorld() || !GetWorld()->IsGameWorld()) ratto_return("World is NOT Ready");
	
	// Check if UserID and PlayerCharacter are Valid
	if (UserID <= 0) ratto_return("UserID is Invalid");
			
	// Find from TMap
	if (List_PlayerCharacters.Contains(UserID))
	{
		// Remove from TMap
		List_PlayerCharacters.Remove(UserID);
	}
	else ratto_return("UserID is not Found in TMap, Failed to Remove");
}
