#include "PLAYER/C_PlayerController.h"
#include "SHARED_FUNC_LIB.h"

#include "GAME/C_GameInstance.h"

#include "PLAYER/C_PlayerState.h"
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
	}
}

void AC_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check if Local Controller
	if (IsLocalController())
	{
		// Prevent Crash
		if (!IsValid(Ref_ClientSystems))
		{
			// Check if Pawn has Spawned
			if (IsValid(GetPawn()))
			{
				// Prevent Crash
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
			
				// Check if GameInstance is Valid
				if (IsValid(GetGameInstance()))
				{
					// Set the GameInstance Reference
					Ref_ClientSystems->Ref_GameInstance = Cast<UC_GameInstance>(GetGameInstance());
					// Get the Camera Settings
					Ref_ClientSystems->Get_Camera_Settings();
				}
				else ratto_return("GAME INSTANCE IS NOT READY");
			}
			else ratto_return("PAWN is NOT READY");
		}
		
		// Start Camera Systems
		if (!Ref_ClientSystems->bCamera_Settings_Set) ratto_return("bCamera_Settings_Set is False");
		
		// Set Camera Length & Rotation
		Ref_ClientSystems->Set_Camera_Zoom(DeltaTime);
		Ref_ClientSystems->Set_Camera_Rotation(DeltaTime);
	}
}

void AC_PlayerController::Create_ClientSystems_ActorComponent()
{
	Ref_ClientSystems = NewObject<UC_ClientSystems_AC>(this, Ref_ClientSystems_Class);
	AddInstanceComponent(Ref_ClientSystems);
	Ref_ClientSystems->RegisterComponent();
	Ref_ClientSystems->Ref_PlayerController = this;
	Ref_ClientSystems->bThisComponentReady = true;
}