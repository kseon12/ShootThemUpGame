// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUPTEST_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:

    ASTURifleWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;
	virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.f;

    /**
     * The half radius of cone
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

private:
    FTimerHandle ShotTimerHandle;

    void MakeDamage(FHitResult& HitResult);

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Value);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
