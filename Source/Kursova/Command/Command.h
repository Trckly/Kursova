#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class KURSOVA_API UCommand : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* Actor) const PURE_VIRTUAL(UCommand::Execute,);
};
