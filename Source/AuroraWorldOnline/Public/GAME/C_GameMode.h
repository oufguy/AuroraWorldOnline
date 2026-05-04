#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PLAYER/C_PlayerCharacter.h"

class AC_PlayerCharacter;

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
	
	// ========================================================================
	// PLAYER SYSTEM
	// ========================================================================
	
	UPROPERTY()
	TMap<int32, TObjectPtr<AC_PlayerCharacter>> List_PlayerCharacters;
	
	UFUNCTION()
	AC_PlayerCharacter* List_PlayerCharacters_Find(int32 UserID);
	UFUNCTION()
	void List_PlayerCharacters_Add(int32 UserID, AC_PlayerCharacter *New_PlayerCharacter);
	UFUNCTION()
	void List_PlayerCharacters_Remove(int32 UserID);
	
};
