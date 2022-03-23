// I will find u. Copyrighted

#pragma once

#include "STUCoreTypes.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"

class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUPTEST_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
