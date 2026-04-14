#include "GAME/C_GameMode.h"
#include "SHARED_FUNC_LIB.h"

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
	
	if (!NewPlayer) ratto_return("NewPlayer is Invalid");
}
