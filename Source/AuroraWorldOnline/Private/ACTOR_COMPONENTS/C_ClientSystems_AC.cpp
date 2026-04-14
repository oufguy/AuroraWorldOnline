#include "ACTOR_COMPONENTS/C_ClientSystems_AC.h"
#include "SHARED_FUNC_LIB.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GAME/C_GameInstance.h"

#include "PLAYER/C_PlayerState.h"
#include "PLAYER/C_PlayerController.h"
#include "PLAYER/C_PlayerSoul.h"

#include "PLAYER/C_PlayerCharacter.h"


// ========================================================================
// SHARED
// ========================================================================

UC_ClientSystems_AC::UC_ClientSystems_AC()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void UC_ClientSystems_AC::BeginPlay()
{
	Super::BeginPlay();
}

void UC_ClientSystems_AC::StartTimerManager()
{
}

void UC_ClientSystems_AC::TimerRepeatFunction()
{
}

void UC_ClientSystems_AC::StartLoopManager()
{
}

void UC_ClientSystems_AC::LoopRepeatFunction()
{
}

// ========================================================================
// INPUT SYSTEM
// ========================================================================

void UC_ClientSystems_AC::Add_InputMappingContext()
{
	// Get the Owning SubSystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Ref_PlayerController->GetLocalPlayer());
    // Prevent Null
	if (!Subsystem) ratto_return("SubSystem is Invalid");
	
	// Clear ALL
	Subsystem->ClearAllMappings();
	// Add the Mapping Context
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	
	// Set True to Prevent Loop
	bInputMapping_Ready = true;
}

void UC_ClientSystems_AC::Bind_InputMappingKeys()
{
	// Get the Input Component
	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(Ref_PlayerController->InputComponent);
	if (!InputComponent) ratto_return("Input Component is Invalid");
	
	// Bind the Action Keys
	if (!bInputActions_Ready)
	{
		InputComponent->BindAction(IA_Mouse_LeftClick, ETriggerEvent::Triggered, this, &UC_ClientSystems_AC::Action_Mouse_LeftClick);
		///////////////////////////////////////////////////////////////////////////////////
		InputComponent->BindAction(IA_Mouse_RightClick, ETriggerEvent::Triggered, this, &UC_ClientSystems_AC::Action_Mouse_RightClick);
		InputComponent->BindAction(IA_Mouse_RightClick, ETriggerEvent::Canceled, this, &UC_ClientSystems_AC::Action_Mouse_RightClick);
		InputComponent->BindAction(IA_Mouse_RightClick, ETriggerEvent::Completed, this, &UC_ClientSystems_AC::Action_Mouse_RightClick);
		///////////////////////////////////////////////////////////////////////////////////
		InputComponent->BindAction(IA_Mouse_WheelAxis, ETriggerEvent::Triggered, this, &UC_ClientSystems_AC::Action_Mouse_WheelAxis);
		///////////////////////////////////////////////////////////////////////////////////
		InputComponent->BindAction(IA_InteractionMode, ETriggerEvent::Triggered, this, &UC_ClientSystems_AC::Action_IA_InteractionMode);
		///////////////////////////////////////////////////////////////////////////////////
		InputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UC_ClientSystems_AC::Action_Movement);
		
		// Set True to Prevent Loop
		bInputActions_Ready = true;
	}
}

void UC_ClientSystems_AC::Action_Mouse_LeftClick(const FInputActionValue& Value)
{
}

void UC_ClientSystems_AC::Action_Mouse_RightClick(const FInputActionInstance& Instance)
{
}

void UC_ClientSystems_AC::Action_Mouse_WheelAxis(const FInputActionValue& Value)
{
}

void UC_ClientSystems_AC::Action_Movement(const FInputActionValue& Value)
{
}

void UC_ClientSystems_AC::Action_IA_InteractionMode(const FInputActionValue& Value)
{
}


// ========================================================================
// UI SYSTEM
// ========================================================================


//==============================================================
// MOUSE SYSTEM
//==============================================================

void UC_ClientSystems_AC::Set_MouseInputMode(EMouse_InputMode MouseInputMode)
{
	Current_MouseInputMode = MouseInputMode;
	
	switch (MouseInputMode)
	{
	// Game Only, Preferably for Rotating Camera
	case EMouse_InputMode::GameOnly:
		{
			FInputModeGameOnly InputMode;
			InputMode.SetConsumeCaptureMouseDown(true);

			Ref_PlayerController->SetInputMode(InputMode);
			Ref_PlayerController->SetShowMouseCursor(false);
			break;
		}
		// Game + UI, Preferably for Normal Play
	case EMouse_InputMode::GameAndUI:
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			Ref_PlayerController->SetInputMode(InputMode);
			Ref_PlayerController->SetShowMouseCursor(true);
			break;
		}
	}
}

EMouse_InputMode UC_ClientSystems_AC::Get_MouseInputMode() const
{
	return Current_MouseInputMode;
}

FHitResult UC_ClientSystems_AC::Get_MouseHit(ECollisionChannel CollisionChannel) const
{
	// Variables
	FHitResult CursorHit;

	// Cursor Hit Function
	if (Ref_PlayerController->GetHitResultUnderCursor(CollisionChannel, false, CursorHit))
	{
		// Prevent Null & Crash
		if (!CursorHit.bBlockingHit) return FHitResult();

		// Send the hit results
		return CursorHit;
	}
	else
	{
		return FHitResult();
	}
}

FVector UC_ClientSystems_AC::Get_MouseLocation(bool bDebug, float MaxTraceDistance) const
{
	// Get CursorHit
	FHitResult CursorHit = Get_MouseHit(Highlight_SendRequest.Highlight_CollisionChannel);

	// Prevent Null & Crash
	if (!CursorHit.bBlockingHit) return FVector::ZeroVector;
		
	// Hit Lines
	FVector HitLocation = CursorHit.Location;
		
	// Debug Part
	if (bDebug)
	{
		// Trace Variables
		FVector TraceStart = CursorHit.TraceStart;
		FVector TraceEnd = CursorHit.TraceEnd;
		
		// Clamp and Set TrueEnd
		FVector Direction = (TraceEnd - TraceStart).GetSafeNormal();
		TraceEnd = TraceStart + (Direction * MaxTraceDistance); // Updated
				
		// Debug Variables
		FColor LineColor = FColor::Red;
		float Duration = 5.0f;
		float Thickness = 1.0f;

		// Draw Debug Line
		UKismetSystemLibrary::DrawDebugLine(
			GetWorld(),
			TraceStart,
			TraceEnd,
			LineColor,
			Duration,
			Thickness
		);
	}

	// Send the hit location
	return HitLocation;
}

//==============================================================
// HIGHLIGHT SYSTEM
//==============================================================

void UC_ClientSystems_AC::Set_HighlightTarget()
{
	// Get CursorHit
	FHitResult CursorHit = Get_MouseHit(Highlight_SendRequest.Highlight_CollisionChannel);
	
	// Prevent Null & Crash
	if (!CursorHit.bBlockingHit)
	{
		Clear_PreviousHighlights();
		return;
	}
	else if (CursorHit.bBlockingHit)
	{
		// Get Target
		AActor * TargetActor = CursorHit.GetActor();
		
		// Prevent Null & Crash
		if (!TargetActor) return;
		
		// if Target is Faded, do not Highlight
		if (Actors_Faded.Contains(TargetActor))
		{
			Clear_PreviousHighlights();
			return;
		};
		
		// Check if Target Already Exists
		if (Actors_Highlighted.Contains(TargetActor)) return;
		// if Target doesn't exist, Clean Previous
		else if (!Actors_Highlighted.Contains(TargetActor)) Clear_PreviousHighlights();
    		
		// Get Target Components
		TArray<UPrimitiveComponent*> TargetComponents;
		TargetActor->GetComponents(TargetComponents);
    		
		// Get Components by Order
		for (UPrimitiveComponent* TargetComp : TargetComponents)
		{
			// Prevent Null & Crash
			if (!IsValid(TargetComp)) continue;
    		
			// Set OverlayMaterial to New Targets
			if (USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(TargetComp))
			{
				SkeletalMesh->SetOverlayMaterial(Highlight_SendRequest.MaterialToUse);
			}
			else if (UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(TargetComp))
			{
				StaticMesh->SetOverlayMaterial(Highlight_SendRequest.MaterialToUse);
			}
		}
    		
		// Add Target
		Actors_Highlighted.Add(TargetActor);
	}
}

void UC_ClientSystems_AC::Clear_PreviousHighlights()
{
	// Prevent Unnecessary and Crash
	if (Actors_Highlighted.Num() == 0) return;
	
	// Get Every Target by Order
	for (int i = Actors_Highlighted.Num() - 1 ; i >= 0 ;i--)
	{
		// Get Actor
		AActor * TargetActor = Actors_Highlighted[i];
		// Prevent Null & Crash
		if (!TargetActor) return;
			
		// Get Target Components
		TArray<UPrimitiveComponent*> TargetComponents;
		TargetActor->GetComponents(TargetComponents);
    			
		// Get Components by Order
		for (UPrimitiveComponent* TargetComp : TargetComponents)
		{
			// Prevent Null & Crash
			if (!IsValid(TargetComp)) continue;
    				
			// Clear OverlayMaterial from Previous Targets
			if (USkeletalMeshComponent * SkeletalMesh = Cast<USkeletalMeshComponent>(TargetComp))
			{
				SkeletalMesh->SetOverlayMaterial(nullptr);
			}
			else if (UStaticMeshComponent * StaticMesh = Cast<UStaticMeshComponent>(TargetComp))
			{
				StaticMesh->SetOverlayMaterial(nullptr);
			}
		}
    			
		// Remove Target
		Actors_Highlighted.RemoveAt(i);
	}
	
}

//==============================================================
// CAMERA SYSTEM
//==============================================================

void UC_ClientSystems_AC::Create_PlayerCameraComponent()
{
	// Prevent Crash & Null
	if (!IsValid(Ref_PlayerSoul))
	{
		RLOG_E("PLEASE SET Ref_PlayerSoul")
		return;
	}
	
	// Create the SpringArmComponent
	SpringArm = NewObject<USpringArmComponent>(Ref_PlayerSoul, USpringArmComponent::StaticClass(), TEXT("SpringArm"));
	if (SpringArm)
	{
		SpringArm->RegisterComponent();
		SpringArm->AttachToComponent(Ref_PlayerSoul->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SpringArm->TargetArmLength = 500.f;
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->SetRelativeLocation(FVector::ZeroVector);
	}
	else
	{
		RLOG_E("SpringArm IS NOT Ready")
		return;	
	}

	// Create the CameraComponent
	Camera = NewObject<UCameraComponent>(Ref_PlayerSoul, UCameraComponent::StaticClass(), TEXT("Camera"));
	if (Camera)
	{
		Camera->RegisterComponent();
		if (SpringArm)
		{
			Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
			Camera->Activate();
		}
		Camera->bUsePawnControlRotation = false;
	}
	else
	{
		
		UE_LOG(LogTemp, Error, TEXT("UC_ClientSystems_AC::Create_PlayerCameraComponent() :: Camera IS NOT Ready"))
		return;
	}
	
	// Set the View Target so Camera Works
	Ref_PlayerController->SetViewTargetWithBlend(Ref_PlayerSoul);
}


TArray<FHitResult> UC_ClientSystems_AC::Get_CameraHit(ETraceTypeQuery TraceChannel, EDrawDebugTrace::Type DrawDebugType) const
{
	// Start Location
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = Ref_PlayerSoul->GetActorLocation();
	
	TArray<FHitResult> HitResult;
	TArray<AActor*> ObjectsToFade;

	// Hit System
	UKismetSystemLibrary::SphereTraceMulti(
		this,
		StartLocation,
		EndLocation,
		15.0f,   // Sphere radius = small tight line-of-sight bubble
		TraceChannel,
		true,
		TArray<AActor*>(),
		DrawDebugType,
		HitResult,
		false
	);
	
	// Send hit Results
	if (HitResult.Num() > 0) return HitResult;
	else return TArray<FHitResult>();
}

FRotator UC_ClientSystems_AC::Get_SpringArmRotation() const
{
	// Get SpringArm Rotation
	FRotator SAControlRotation = SpringArm->GetRelativeRotation();
	
	SAControlRotation.Pitch = 0.0f;
	SAControlRotation.Roll  = 0.0f;
	// Set WorldDirections
	if (SAControlRotation != FRotator::ZeroRotator) return SAControlRotation;
	else return FRotator::ZeroRotator;
}

void UC_ClientSystems_AC::Set_isRotating(bool isRotating)
{
	if (isRotating)
	{
		// Create Temporary Values
		float MouseX, MouseY;

		// Update Values
		if (Ref_PlayerController->GetMousePosition(MouseX, MouseY))
		{
			// Save it
			Save_MousePosition(FVector2D(MouseX, MouseY));
		}
		
		// Make it Rotate-able
		Set_MouseInputMode(EMouse_InputMode::GameOnly);
		bRotatingCamera = true;
	}
	else
	{
		// Reset the Mouse Pos
		Reset_MousePosition();
		
		// Reset to Normal
		Set_MouseInputMode(EMouse_InputMode::GameAndUI);
		bRotatingCamera = false;
	}
	
}

bool UC_ClientSystems_AC::Get_isRotating() const
{
	return bRotatingCamera;
}

void UC_ClientSystems_AC::Save_MousePosition(const FVector2D New_MousePosition)
{
	
	// Set it to Before Rotation Position
	Last_MousePosition = New_MousePosition;
}

void UC_ClientSystems_AC::Reset_MousePosition() const
{
	
	// Set it to Before Rotation Position
	Ref_PlayerController->SetMouseLocation(Last_MousePosition.X, Last_MousePosition.Y);
}

void UC_ClientSystems_AC::Set_Camera_Settings()
{
	// Create PlayerCamera
	Create_PlayerCameraComponent();
	
	// Set Camera_Settings
	Camera_Settings = &Ref_GameInstance->Camera_Settings;
	bCamera_Settings_Set = true;
}

void UC_ClientSystems_AC::Set_Camera_Values(float DeltaTime)
{
	// Prevent Unnecessary Action
	if (Camera_Values.Current_Zoom != Camera_Values.Target_Zoom)
	{
		// Check if Close
		if (!FMath::IsNearlyEqual(Camera_Values.Current_Zoom, Camera_Values.Target_Zoom, 0.1f))
		{
			// Apply
			Camera_Values.Current_Zoom = FMath::FInterpTo(
				Camera_Values.Current_Zoom,
				Camera_Values.Target_Zoom,
				DeltaTime,
				Camera_Settings->Zoom_Speed
			);
			SpringArm->TargetArmLength = Camera_Values.Current_Zoom;
		}
		// if Close, Apply
		else
		{
			// Apply
			Camera_Values.Current_Zoom = Camera_Values.Target_Zoom;
			SpringArm->TargetArmLength = Camera_Values.Current_Zoom;
		}
	}

	// Prevent Unnecessary Action
	if (Camera_Values.Current_Horizontal != Camera_Values.Target_Horizontal)
	{
		// Apply
		Camera_Values.Current_Horizontal = Camera_Values.Target_Horizontal;
	}
	
	// Prevent Unnecessary Action
	if (Camera_Values.Current_Vertical != Camera_Values.Target_Vertical)
	{
		// Apply
		Camera_Values.Current_Vertical = Camera_Values.Target_Vertical;
	}
	
	// Lastly, Apply Rotation
	SpringArm->SetRelativeRotation(FRotator(Camera_Values.Current_Vertical, Camera_Values.Current_Horizontal, 0.0f));
}

void UC_ClientSystems_AC::Add_CameraZoom(float ZoomRate)
{
	// Prevent 0
	if (ZoomRate == 0.0f) return;
	
	// Inversion Check
	if (Camera_Settings->Zoom_Inverted == false)
	{
		ZoomRate *= -1;
	}
	
	// Apply
	Camera_Values.Target_Zoom = FMath::Clamp(
		Camera_Values.Target_Zoom
		+ (Camera_Settings->Zoom_Sensitivity * ZoomRate),
		Camera_Values.Min_Zoom,
		Camera_Values.Max_Zoom
	);
}

void UC_ClientSystems_AC::Add_CameraRotation(float HorizontalRate, float VerticalRate)
{
	// Prevent 0
	if (VerticalRate == 0.0f && HorizontalRate == 0.0f) return;

	// Prevent Unnecessary Action
	if (HorizontalRate >= 1.5f || HorizontalRate <= -1.5f)
	{
		// Inversion Check
		if (Camera_Settings->Horizontal_Inverted == false)
		{
			HorizontalRate *= -1;
		}
		
		// Apply
		Camera_Values.Target_Horizontal += (Camera_Settings->Horizontal_Sensitivity *HorizontalRate);		
	}
	
	// Prevent Unnecessary Action
	if (VerticalRate >= 1.5f || VerticalRate <= -1.5f)
	{
		// Inversion Check
		if (Camera_Settings->Vertical_Inverted == false)
		{
			VerticalRate *= -1;
		}
		
		// Apply
		Camera_Values.Target_Vertical = FMath::Clamp(
			Camera_Values.Target_Vertical
			+ (Camera_Settings->Vertical_Sensitivity * VerticalRate),
			Camera_Values.Min_Vertical,
			Camera_Values.Max_Vertical
		);
	}
}

//==============================================================
// FADE SYSTEM
//==============================================================

void UC_ClientSystems_AC::Request_FadeTarget()
{
	// Get HitResult
	TArray<FHitResult> HitResult = Get_CameraHit(Fade_SendRequest.Fade_TraceChannel);
	TArray<AActor*> ActorsToFade = TArray<AActor*>();
	
	// Clear List if No Targets are Hit
	if (HitResult.Num() == 0)
	{
		Clear_PreviousFades();
		return;
	}
	else if (HitResult.Num() > 0)
	{
		// Get Hit Target Actors by Order 
		for (int i = 0; i < HitResult.Num(); i++)
		{
			// Get Hit Actors
			AActor * HitActor = HitResult[i].GetActor();
			
			// Prevent Null & Crash
			if (!HitActor) continue;
			// Skip These Actors
			if (HitActor == Ref_PlayerSoul) continue;
			// Get Character Class
			if (HitActor->IsA(AC_PlayerCharacter::StaticClass()))
			{
				// Get Locations of Camera and Target
				const FVector CameraLocation = Camera->GetComponentLocation();
				const FVector TargetLocation  = HitActor->GetActorLocation();

				// Calculate Distance
				const float Distance = FVector::Dist(CameraLocation, TargetLocation);
				
				// Skip if not too close
				if (Distance > Camera_Settings->Fade_CameraBlock_Dist) continue;
			}
			
			// Check if Already Exists
			if (ActorsToFade.Contains(HitActor)) continue;
			
			// Add Unique Target to Array
			ActorsToFade.AddUnique(HitActor);
		}
		
		// Get Target Actors by Order
		for (int i = 0; i < ActorsToFade.Num(); i++)
		{
			// Get Target Actors
			AActor * TargetActor = ActorsToFade[i];
			// Prevent Null & Crash
			if (!IsValid(TargetActor)) continue;
			
			// Check if TargetComponent Exists
			UC_FadeComponent* FadeComp =
				Cast<UC_FadeComponent>(TargetActor->GetComponentByClass(UC_FadeComponent::StaticClass()));

			// if Doesn't Exist, add and run
			if (!FadeComp)
			{
				// Add the ActorComponent
				FadeComp = NewObject<UC_FadeComponent>(TargetActor);
				FadeComp->RegisterComponent();
				
				// Modify ActorComponent
				FadeComp->FadeMaterial = Fade_SendRequest.FadeMaterial;
				FadeComp->FadeParameterName = Fade_SendRequest.FadeParameterName;
				FadeComp->AnimationTime = Fade_SendRequest.AnimationTime;
				
				// Run the Function to Fade
				FadeComp->Request_Fade(true);
				
				// Add the Target to the List
				Actors_Faded.AddUnique(TargetActor);
			}
		}
		
		// Clear the List
		Clear_PreviousFades(ActorsToFade);
	}
	
}

void UC_ClientSystems_AC::Clear_PreviousFades(const TArray<AActor*>& ActorsToFade)
{
	// Check if There are any Targets
	if (Actors_Faded.Num() == 0) return;
	
	// Go by Order to Check Faded Actors
	for (int i = Actors_Faded.Num() - 1; i >= 0 ; i--)  // Start at Num() - 1, not Num()
	{
		// Get Target Actor
		AActor * FadedActor = Actors_Faded[i];
		// Prevent Null & Crash
		if (!FadedActor) continue;
		
		// Check if Target still exists
		if (ActorsToFade.Contains(FadedActor)) continue;
		
		// Get FadeComponent to clear the fade
		UC_FadeComponent* FadeComp =
			Cast<UC_FadeComponent>(FadedActor->GetComponentByClass(UC_FadeComponent::StaticClass()));
		
		// if Exists, clear it
		if (FadeComp)
		{
			FadeComp->Request_Fade(false);
		}
		
		// Remove Target
		Actors_Faded.Remove(FadedActor);
	}
}
	
// ========================================================================
// MOVEMENT SYSTEM
// ========================================================================
