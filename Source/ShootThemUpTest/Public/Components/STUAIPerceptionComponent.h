// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
};
