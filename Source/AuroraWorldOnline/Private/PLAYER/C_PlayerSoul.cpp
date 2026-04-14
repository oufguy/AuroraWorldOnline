#include "PLAYER/C_PlayerSoul.h"
#include "SHARED_FUNC_LIB.h"

// ========================================================================
// SHARED
// ========================================================================

AC_PlayerSoul::AC_PlayerSoul()
{
	PrimaryActorTick.bCanEverTick = true;
	// No Need to replicate yourself to others
	bReplicates = false;
}

void AC_PlayerSoul::BeginPlay()
{
	Super::BeginPlay();
}

void AC_PlayerSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

