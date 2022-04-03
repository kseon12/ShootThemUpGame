// I will find u. Copyrighted

#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "KIsmet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");

}

void ASTURifleWeapon::StartFire()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Weapon %s started to fire"), *GetName());

	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Weapon %s fire stoped"), *GetName());

	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::Zoom(bool Enabled)
{
	//Super::Zoom(Enabled);

	const auto PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController || !PlayerController->PlayerCameraManager ) return;

	if(Enabled)
	{
		DefaultCameraPOV= PlayerController->PlayerCameraManager->GetFOVAngle();
	}
	
	PlayerController->PlayerCameraManager->SetFOV(Enabled? FOVZoomAngle: DefaultCameraPOV);
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Weapon %s shot calculation"), *GetName());

	if(!GetWorld() || IsAmmoEmpty()) return;

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if(HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	return true;
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
	auto DamagedCharacter = Cast<ASTUBaseCharacter>(HitResult.GetActor());
	if(!DamagedCharacter) return;

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamagedCharacter->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if(!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	if(!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSoundCue,WeaponMesh, MuzzleFlashSocketName);
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Value)
{
	if(MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Value);
		MuzzleFXComponent->SetVisibility(Value, true);
	}

	if(FireAudioComponent)
	{
		Value ? FireAudioComponent->Play() : FireAudioComponent->Stop();
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if(TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* ASTURifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
