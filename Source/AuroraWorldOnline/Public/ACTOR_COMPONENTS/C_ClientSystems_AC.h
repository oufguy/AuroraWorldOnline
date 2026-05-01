#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MOUSE_LIBRARY.h"
#include "CAMERA_LIBRARY.h"
#include "MOVEMENT_LIBRARY.h"

#include "ACTOR_COMPONENTS/C_FadeComponent.h"

#include "InputActionValue.h"

class UInputMappingContext;
class UInputAction;

class UC_GameInstance;

class AC_PlayerState;
class AC_PlayerController;
class AC_PlayerSoul;

#include "C_ClientSystems_AC.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class AURORAWORLDONLINE_API UC_ClientSystems_AC : public UActorComponent
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	UC_ClientSystems_AC();
	
	bool bThisComponentReady = false;
	
	UPROPERTY()
	TObjectPtr<UC_GameInstance> Ref_GameInstance = nullptr;
	
	UPROPERTY()
	TObjectPtr<AC_PlayerController> Ref_PlayerController = nullptr;
	UPROPERTY()
	TObjectPtr<AC_PlayerSoul> Ref_PlayerSoul = nullptr;
	UPROPERTY()
	TObjectPtr<AC_PlayerState> Ref_PlayerState = nullptr;
	
private:
	UPROPERTY()
	TArray<AActor*> Actors_Faded; // FADE SYSTEM
	UPROPERTY()
	TArray<AActor*> Actors_Highlighted; // HIGHLIGHT SYSTEM
	
	virtual void BeginPlay() override;
	
	FTimerHandle TimerHandle;
	void StartTimerManager();
	void TimerRepeatFunction();
	
	FTimerHandle LoopHandle;
	void StartLoopManager();
	void LoopRepeatFunction();
	
	// ========================================================================
	// INPUT SYSTEM
	// ========================================================================
public:
	// Variables to prevent Actions when they aren't ready
	bool bInputMapping_Ready = false;

	void Add_InputMappingContext();
	bool bInputActions_Ready = false;

	void Bind_InputMappingKeys();
	
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | MAPPING")
	UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | IACTIONS")
	UInputAction* IA_Mouse_LeftClick;
	UFUNCTION()
	void Action_Mouse_LeftClick(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | IACTIONS")
	UInputAction* IA_Mouse_RightClick;
	UFUNCTION()
	void Action_Mouse_RightClick(const FInputActionInstance& Instance);
	
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | IACTIONS")
	UInputAction* IA_Mouse_WheelAxis;
	UFUNCTION()
	void Action_Mouse_WheelAxis(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | IACTIONS")
	UInputAction* IA_Movement;
	UFUNCTION()
	void Action_Movement(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | INPUT | IACTIONS")
	UInputAction* IA_InteractionMode;
	UFUNCTION()
	void Action_IA_InteractionMode(const FInputActionValue& Value);
	
	// ========================================================================
	// UI SYSTEM
	// ========================================================================
	

	// ========================================================================
	// MOUSE SYSTEM
	// ========================================================================
public:
	// Current Mouse Input Mode to Check
	EMouse_InputMode Current_MouseInputMode;
	bool bUIOnlyMode = false;
	
	void Set_MouseInputMode(EMouse_InputMode MouseInputMode);
	
	FHitResult Get_MouseHit(ECollisionChannel CollisionChannel = ECC_Visibility) const;
	FVector Get_MouseLocation(bool bDebug, float MaxTraceDistance) const;
	
private:
	FVector2D Last_MousePosition = FVector2D(0, 0);
	void Save_MousePosition(const FVector2D New_MousePosition);
	void Reset_MousePosition() const;
	
	// ========================================================================
	// HIGHLIGHT SYSTEM
	// ========================================================================
public:
	UPROPERTY(EditDefaultsOnly, Category="SETTINGS | HIGHLIGHT")
	FHighlight_SendRequest Highlight_SendRequest;

	void Set_HighlightTarget();
private:
	void Clear_PreviousHighlights();
	
	// ========================================================================
	// CAMERA SYSTEMS
	// ========================================================================
public:
	////////// SHARED //////////
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera = nullptr;
	
	void Create_PlayerCameraComponent();

	FCamera_Values Camera_Values;
	FCamera_Settings *Ref_Camera_Settings;
	bool bCamera_Settings_Set = false;
	void Get_Camera_Settings();
	
private:
	TArray<FHitResult> Get_CameraHit(ETraceTypeQuery TraceChannel, EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None) const;
	
public:
	////////// ZOOM //////////
	void Add_Camera_Zoom(float ZoomRate);
	void Set_Camera_Zoom(float DeltaTime);
	
	////////// ROTATION //////////
	FRotator Get_SpringArmRotation() const; // CAMERA Direction for World Direction
	
	bool bRotatingCamera = false;
	void Set_isRotating(bool isRotating);
	
	void Add_Camera_Rotation(float HorizontalRate, float VerticalRate);
	void Set_Camera_Rotation(float DeltaTime);
	
	// ========================================================================
	// FADE SYSTEM
	// ========================================================================
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SETTINGS | FADE")
	FFade_SendRequest Fade_SendRequest;
	
	void Request_FadeTarget();
	void Clear_PreviousFades(const TArray<AActor*>& ActorsToFade = TArray<AActor*>());
	
	// ========================================================================
	// MOVEMENT SYSTEM
	// ========================================================================
};
