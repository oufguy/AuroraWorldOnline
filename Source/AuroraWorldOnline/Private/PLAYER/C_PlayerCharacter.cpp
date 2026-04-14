#include "PLAYER/C_PlayerCharacter.h"
#include "SHARED_FUNC_LIB.h"

#include "Net/UnrealNetwork.h"


// ========================================================================
// SHARED
// ========================================================================

AC_PlayerCharacter::AC_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	CameraSocket = CreateDefaultSubobject<USceneComponent>(TEXT("CameraSocket"));
	CameraSocket->SetIsReplicated(false);
	
	CameraSocket->SetRelativeLocation(FVector(0, 0, 70));
	CameraSocket->SetupAttachment(RootComponent);
}

void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_PlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, UserID);
	DOREPLIFETIME(ThisClass, MovementStruct);
}

