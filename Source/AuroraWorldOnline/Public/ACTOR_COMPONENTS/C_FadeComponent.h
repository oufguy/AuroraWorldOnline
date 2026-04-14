#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_FadeComponent.generated.h"

USTRUCT(BlueprintType)
struct FSaveMaterialStruct
{
	GENERATED_BODY()

	UPROPERTY()
	UPrimitiveComponent* Component = nullptr;

	UPROPERTY()
	int32 MaterialIndex = INDEX_NONE;

	UPROPERTY()
	UMaterialInterface* OriginalMaterial = nullptr;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURORAWORLDONLINE_API UC_FadeComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	//==============================================================
	// SHARED
	//==============================================================
	
	// Sets default values for this component's properties
	UC_FadeComponent();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	FTimerHandle TimerHandle;
	void StartTimerManager();
	void EndTimer();

	//==============================================================
	// FADE
	//==============================================================
private:
	UPROPERTY()
	TArray<FSaveMaterialStruct> Saved_Materials;
	
public:
	UPROPERTY(BlueprintReadWrite, Category="Fade System | Variables")
	bool bIsFading = false;
	UPROPERTY(BlueprintReadWrite, Category="Fade System | Variables")
	bool bIsBeingDestroyed = false;

	UPROPERTY(BlueprintReadWrite, Category="Fade System | Variables")
	UMaterialInstance* FadeMaterial = nullptr;
	UPROPERTY(BlueprintReadWrite, Category="Fade System | Variables")
	FName FadeParameterName = "FadeParam";
	
	UPROPERTY(BlueprintReadWrite, Category="Fade System | Variables")
	float AnimationTime = 0.5f;
private:
	float ElapsedFadeTime = 0.0f;

	
public:
	UFUNCTION(BlueprintCallable, Category="Fade System | Functions")
	void Request_Fade(bool FadeActor);

private:
	void Control_FadeSystem();
};
