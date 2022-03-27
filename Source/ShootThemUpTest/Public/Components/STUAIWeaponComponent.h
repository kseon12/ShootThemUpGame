// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Components/STUWeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUAIWeaponComponent : public USTUWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;
};
