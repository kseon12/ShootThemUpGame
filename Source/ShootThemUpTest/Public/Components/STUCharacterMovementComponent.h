// I will find u. Copirighted

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	virtual float GetMaxSpeed() const override;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement",
        meta = (AllowPrivateAccess = "True", ClampMin = "1.5", ClampMax = "10.0"))
    float RunSpeedModifier = 2.0f;
};
