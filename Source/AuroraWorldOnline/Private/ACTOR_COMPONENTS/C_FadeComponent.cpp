// 


#include "ACTOR_COMPONENTS/C_FadeComponent.h"


// Sets default values for this component's properties
UC_FadeComponent::UC_FadeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}


// Called when the game starts
void UC_FadeComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UC_FadeComponent::StartTimerManager()
{
	if (UWorld* World = GetWorld())
	{
		// Start delayed setup attempts
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UC_FadeComponent::Control_FadeSystem,
			0.05f,
			true
		);
	}
}

void UC_FadeComponent::EndTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void UC_FadeComponent::Request_Fade(bool FadeActor)
{
	// Get Owning Actor that Owns this Component
	AActor* TargetActor = GetOwner();
	// Prevent Null & Crash
	if (!IsValid(TargetActor)) return;
	
	// if Fade is True
	if (FadeActor)
	{
		// Prevent Re-Apply
		if (bIsFading) return;
		if (bIsBeingDestroyed) return;
		
		// Make it TRUE to prevent Loop
		bIsFading = true;
        
        // Reset elapsed time
        ElapsedFadeTime = 0.0f;

		// Prevent Null & Crash
		if (!FadeMaterial) return;

		// Clean Saved Materials
		Saved_Materials.Empty();

		// Get Target Actor Components
		TArray<UPrimitiveComponent*> TargetComponents;
		TargetActor->GetComponents(TargetComponents);

		// Get Components by Order
		for (UPrimitiveComponent* TargetComp : TargetComponents)
		{
			// Prevent Null & Crash
			if (!IsValid(TargetComp)) continue;

			// Get Number of Materials in Slots
			const int32 MatCount = TargetComp->GetNumMaterials();

			// Get Materials by Order
			for (int32 m = 0; m < MatCount; m++)
			{
				// Get Struct before Defining
				FSaveMaterialStruct SaveMaterialStruct;

				// Set the related Struct variables to Save Materials
				SaveMaterialStruct.Component = TargetComp;
				SaveMaterialStruct.MaterialIndex = m;
				SaveMaterialStruct.OriginalMaterial = TargetComp->GetMaterial(m);

				// Save to the TArray
				Saved_Materials.Add(SaveMaterialStruct);

				// Lastly Override Material
				TargetComp->SetMaterial(m, FadeMaterial);
			}
		}
		
		// Start Fade from here on
		StartTimerManager();
	}
	// if Fade is off
	else
	{
		// Prevent Loop
		if (bIsBeingDestroyed) return;
		
		// Make it TRUE to prevent Loop
		bIsBeingDestroyed = true;
		
		// Stop Timer
		EndTimer();
		
		// Get the Structs inside TArray by Order
		for (const FSaveMaterialStruct& SaveMaterialStruct : Saved_Materials)
		{
			// Check if Materials still exists
			if (IsValid(SaveMaterialStruct.Component))
			{
				// Restore the Materials of Component
				SaveMaterialStruct.Component->SetMaterial(
					SaveMaterialStruct.MaterialIndex,
					SaveMaterialStruct.OriginalMaterial
				);
			}
		}
		
		// Destroy Component
		this->DestroyComponent();
	}
}

void UC_FadeComponent::Control_FadeSystem()
{
    // Get Owning Actor that Owns this Component
    AActor* TargetActor = GetOwner();
    // Prevent Null & Crash
    if (!IsValid(TargetActor)) return;
    
    // Fade the Target
    if (bIsFading)
    {
        // Increment elapsed time (0.05f is your timer interval)
        ElapsedFadeTime += 0.05f;
        
        // Calculate alpha (0 to 1)
        const float Alpha = FMath::Clamp(ElapsedFadeTime / AnimationTime, 0.0f, 1.0f);
        
        // Get Target Actor Components
        TArray<UPrimitiveComponent*> TargetComponents;
        TargetActor->GetComponents(TargetComponents);
        
        // Get Components by Order
        for (UPrimitiveComponent* TargetComp : TargetComponents)
        {
            // Prevent Null & Crash
            if (!IsValid(TargetComp)) continue;

            // Get Number of Materials in Slots
            const int32 MatCount = TargetComp->GetNumMaterials();

            // Get Materials by Order
            for (int32 m = 0; m < MatCount; m++)
            {
                UMaterialInterface* BaseMat = TargetComp->GetMaterial(m);
                if (!BaseMat) continue;

                UMaterialInstanceDynamic* MID =
                    TargetComp->CreateDynamicMaterialInstance(m, BaseMat);
                if (!MID) continue;
                
                // Lerp between min and max using time-based alpha
                float ParameterMin = 0.f;
                float ParameterMax = 1.f;
                MID->GetScalarParameterSliderMinMax(FadeParameterName, ParameterMin, ParameterMax);
                
                const float NewValue = FMath::Lerp(ParameterMax, ParameterMin, Alpha);
                MID->SetScalarParameterValue(FadeParameterName, NewValue);
            }
        }
        
        // Stop fading when complete
        if (Alpha >= 1.0f)
        {
            EndTimer();
        }
    }
}