#include "PLAYER/C_PlayerController.h"
#include "SHARED_FUNC_LIB.h"

#include "PLAYER/C_PlayerSoul.h"

// ========================================================================
// SHARED
// ========================================================================

AC_PlayerController::AC_PlayerController()
{
}

void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Check if Local Controller
	if (IsLocalController())
	{
		// Check if Pawn has Spawned
		if (IsValid(GetPawn()))
		{
			if (!IsValid(Ref_ClientSystems_Class)) ratto_return("PLEASE SET Ref_ClientSystems_Class");
				
			// Add the Actor Component for Systems
			Create_ClientSystems_ActorComponent();
			
			// Set the Pawn Reference
			Ref_ClientSystems->Ref_PlayerSoul = Cast<AC_PlayerSoul>(GetPawn());
			// Add The Camera
			Ref_ClientSystems->Create_PlayerCameraComponent();
			// Add the Input Map and Actions
			Ref_ClientSystems->Add_InputMappingContext();
			Ref_ClientSystems->Bind_InputMappingKeys();
			
		}
		else ratto_return("PAWN is NOT READY");
	}
}

void AC_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_PlayerController::Create_ClientSystems_ActorComponent()
{
	if (IsValid(Ref_ClientSystems_Class))
	{
		Ref_ClientSystems = NewObject<UC_ClientSystems_AC>(this, Ref_ClientSystems_Class);
		AddInstanceComponent(Ref_ClientSystems);
		Ref_ClientSystems->RegisterComponent();
		Ref_ClientSystems->Ref_PlayerController = this;
		Ref_ClientSystems->bThisComponentReady = true;
	}
	else ratto_return("PLEASE SET Ref_ClientSystems_Class");
}