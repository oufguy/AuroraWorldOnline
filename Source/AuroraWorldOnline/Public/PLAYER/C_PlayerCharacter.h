#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MOVEMENT_LIBRARY.h"

#include "C_PlayerCharacter.generated.h"

UCLASS()
class AURORAWORLDONLINE_API AC_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	AC_PlayerCharacter();
	
	UPROPERTY(Replicated)
	int32 UserID = 0;
	
	UPROPERTY(Replicated)
	FMovementStruct MovementStruct;
	
	UPROPERTY()
	TArray<FVector> CharacterPath;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USceneComponent> CameraSocket;
	
	virtual void BeginPlay() override;
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
