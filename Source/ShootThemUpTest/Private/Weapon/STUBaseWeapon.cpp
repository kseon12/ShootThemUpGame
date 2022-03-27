// I will find u. Copyrighted

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/STUBaseCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

/////////////////////////////////////////////////////////////////////////////////

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("StaticMesh");
	SetRootComponent(WeaponMesh);
}

/////////////////////////////////////////////////////////////////////////////////

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("DefaultAmmo.Bullets can't be ZERO or less"));
	checkf(DefaultAmmo.Clips > 0, TEXT("DefaultAmmo.Clips can't be ZERO or less"));
	CurrentAmmo = DefaultAmmo;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::MakeShot()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("Weapon %s shot calculation"), *GetName());

	/*if (!GetWorld()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

		MakeDamage(HitResult);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}*/
}

/////////////////////////////////////////////////////////////////////////////////

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if(!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto STUCharacter = Cast<ACharacter>(GetOwner());
	if(!STUCharacter) return false;

	if(STUCharacter->IsPlayerControlled())
	{
		const auto PlayerController = GetPlayerController();
		if(!PlayerController) return false;

		PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleFlashSocketName);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleFlashSocketName);
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if(!GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Camera, CollisionQueryParams);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::StartFire()
{
	UE_LOG(LogBaseWeapon, Warning, TEXT("BaseWeapon->StartFire"));
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::StopFire()
{
	UE_LOG(LogBaseWeapon, Warning, TEXT("BaseWeapon->StopFire"));
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::DecreaseAmmo()
{
	if(CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("CurrentAmmo bullets is ZERO. Shouldn't be entered here"));
		return;
	}

	CurrentAmmo.Bullets--;

	if(IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseWeapon::ChangeClip()
{
	if(!CurrentAmmo.bInfinite)
	{
		if(CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("CurrentAmmo clips is ZERO. Shouldn't be entered here"));
			return;
		}

		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

/////////////////////////////////////////////////////////////////////////////////

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFX,
		WeaponMesh,
		MuzzleFlashSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget, true);
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if(CurrentAmmo.bInfinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if(IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if(CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if(DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
