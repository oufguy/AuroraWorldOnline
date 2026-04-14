#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SHARED_FUNC_LIB.generated.h"

// ========================================================================
// LOG SYSTEM
// ========================================================================

// Custom Log Macros
// %s :: %s will output: FunctionName :: Your Message
#define RLOG_E(Format, ...) UE_LOG(LogTemp, Error, TEXT("Owner --> %s --> %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define RLOG_W(Format, ...) UE_LOG(LogTemp, Warning, TEXT("Owner --> %s --> %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define RLOG_I(Format, ...) UE_LOG(LogTemp, Log, TEXT("Owner --> %s --> %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Custom Return with LOG System
#define ratto_return(Msg, ...) \
do { \
RLOG_E(Msg); \
return ##__VA_ARGS__; \
} while (0)

// ========================================================================
// CAST SYSTEM
// ========================================================================

// AUTO SafeCast to Class/Object
#define SAFE_CAST(Obj, Class) USHARED_FUNC_LIB::SafeCastClass(Obj, Class, FString(__FUNCTION__))

////////////////////////////////////////////////
UCLASS()
class AURORA_LIB_API USHARED_FUNC_LIB : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// ========================================================================
	// SHARED
	// ========================================================================
public:
	UFUNCTION(Category="UTILITIES | CASTING")
	static bool SafeCastClass(const UObject* ObjectToGet, const UClass* ClassToCast, FString CallerName = "");
	
	bool bDebug = true;
};
