// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float HealthAmount = 25.0f;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
