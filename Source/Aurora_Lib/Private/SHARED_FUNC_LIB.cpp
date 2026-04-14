#include "SHARED_FUNC_LIB.h"
#include "MAP_PARAMS.h"

bool USHARED_FUNC_LIB::SafeCastClass(const UObject* ObjectToGet, const UClass* ClassToCast, FString CallerName)
{
	if (!IsValid(ObjectToGet))
	{
		RLOG_E("Caller --> %s --> ObjectToGet[1] is nullptr or invalid.", *CallerName);
		return false;
	}

	if (!IsValid(ClassToCast))
	{
		RLOG_E("Caller --> %s --> ClassToCast[2] is nullptr or invalid.", *CallerName);
		return false;
	}

	if (ObjectToGet->IsA(ClassToCast))
	{
		return true;
	}
	
	RLOG_E("Caller --> %s --> SafeCastClass[3] failed: %s is not a %s", *CallerName, *ObjectToGet->GetName(), *ClassToCast->GetName());
	return false;
}

bool UMAP_PARAMS::IsSpawnAble(FString MapName)
{
	if (MapName == TEXT("DEV_MAP"))
		return true;
	else
	{
		RLOG_E("Map Doesn't Exist or Wrong Name : %s", *MapName)
		return false;
	}
}

FMap_SpawnParams UMAP_PARAMS::Get_MapSpawnPoints(ESpawnMap ChosenMap, int8 SpawnPoint_No)
{
	FMap_SpawnParams Result_SpawnParams;

	switch (ChosenMap)
	{
	case ESpawnMap::DEV_MAP:
		if (SpawnPoint_No == 1)
		{
			Result_SpawnParams.Location = FVector(0.f, 0.f, 100.f);
			Result_SpawnParams.Rotation = FRotator::ZeroRotator;
		}
		else if (SpawnPoint_No == 2)
		{
			Result_SpawnParams.Location = FVector(100.f, 100.f, 100.f);
			Result_SpawnParams.Rotation = FRotator::ZeroRotator;
		}
		else if (SpawnPoint_No == 3)
		{
			Result_SpawnParams.Location = FVector(-100.f, -100.f, 100.f);
			Result_SpawnParams.Rotation = FRotator::ZeroRotator;
		}
		break;

	case ESpawnMap::NONE:
	default:
		// Safe default so you don't spawn under the floor
		Result_SpawnParams.Location = FVector(0.f, 0.f, 100.f);
		Result_SpawnParams.Rotation = FRotator::ZeroRotator;
		break;
	}

	return Result_SpawnParams;
}