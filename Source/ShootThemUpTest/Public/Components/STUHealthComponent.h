// I will find u. Copirighted

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float)

class UCameraShakeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUPTEST_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bAutoHeal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float AutoHealStartDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float AutoHealUpdateTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float AutoHealModifier = 0.1f;

	float TimeSinceLastDmg = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

private:
	float Health = 0.0f;
	FTimerHandle AutoHealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void AutoHealUpdate();
	void SetHealth(float Value);

	void PlayCameraShake();
};
