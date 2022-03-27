// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldBeTaken.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API UEnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
