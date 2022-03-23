// I will find u. Copyrighted

#include "Components/STUHealthComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	if(Damage <= 0.0f || IsDead() || !GetWorld()) return;
	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if(IsDead())
	{
		OnDeath.Broadcast();
	}
	else if(bAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoHealTimerHandle, this, &USTUHealthComponent::AutoHealUpdate, AutoHealUpdateTime, true, AutoHealStartDelay);
	}

	PlayCameraShake();
}

void USTUHealthComponent::AutoHealUpdate()
{
	SetHealth(Health + AutoHealModifier);

	if(IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float Value)
{
	const auto NextHealth = FMath::Clamp(Value, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, NextHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if(!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

float USTUHealthComponent::GetHealthPercent() const
{
	if(MaxHealth <= 0)
	{
		UE_LOG(LogHealthComponent, Error, TEXT("GetHealthPercent(): MaxHealth become less or equivalent ZERO"));
		checkNoEntry();
	}

	return Health / MaxHealth;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsDead() || IsHealthFull()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}
