// I will find u. Copyrighted

#include "Components/STUHealthComponent.h"
#include "STUGameModeBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "TimerManager.h"

#include "Perception/AISense_Damage.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

/////////////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{

}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::AutoHealUpdate()
{
	SetHealth(Health + AutoHealModifier);

	if(IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::SetHealth(float Value)
{
	const auto NextHealth = FMath::Clamp(Value, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if(!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::Killed(AController* KillerController)
{
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController,VictimController);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if(Damage <= 0.0f || IsDead() || !GetWorld()) return;
	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if(IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if(bAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoHealTimerHandle, this, &USTUHealthComponent::AutoHealUpdate, AutoHealUpdateTime, true, AutoHealStartDelay);
	}

	PlayCameraShake();

	ReportDamageEvent(Damage,InstigatedBy);
}

/////////////////////////////////////////////////////////////////////////////////

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character =Cast<ACharacter>(DamagedActor);
	if(!Character) return 1.0f;

	const auto PhysMaterial= Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if(!DamageModifiers.Contains(PhysMaterial)) return 1.0f;

	return DamageModifiers[PhysMaterial];
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(),InstigatedBy->GetPawn(),Damage, InstigatedBy->GetPawn()->GetActorLocation(),GetOwner()->GetActorLocation());
}

/////////////////////////////////////////////////////////////////////////////////

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
}

/////////////////////////////////////////////////////////////////////////////////

float USTUHealthComponent::GetHealthPercent() const
{
	if(MaxHealth <= 0)
	{
		UE_LOG(LogHealthComponent, Error, TEXT("GetHealthPercent(): MaxHealth become less or equivalent ZERO"));
		checkNoEntry();
	}

	return Health / MaxHealth;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsDead() || IsHealthFull()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor,BoneName);
	ApplyDamage(FinalDamage,InstigatedBy);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage,InstigatedBy);
}

/////////////////////////////////////////////////////////////////////////////////
