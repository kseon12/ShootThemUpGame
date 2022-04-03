// I will find u. Copyrighted

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

#include "GameFramework/Character.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

/////////////////////////////////////////////////////////////////////////////////

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

/////////////////////////////////////////////////////////////////////////////////

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponDataArray.Num() == 2, TEXT("Our Character can hold only 2 weapon items"));

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for(auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::SpawnWeapons()
{
	const auto Owner = Cast<ACharacter>(GetOwner());

	if(!Owner || !GetWorld()) return;

	for(auto WeaponData : WeaponDataArray)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);
		if(!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmpryClip);
		Weapon->SetOwner(GetOwner());
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Owner->GetMesh(), WeaponArmorySocketName);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::StartFire()
{
	if(!CanFire()) return;

	CurrentWeapon->StartFire();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::StopFire()
{
	if(!CurrentWeapon) return;

	CurrentWeapon->StopFire();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::NextWeapon()
{
	if(!CanEquip()) return;

	StopFire();
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& Name)
{
	if(!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, Name);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) return;

	auto Owner = Cast<ACharacter>(GetOwner());
	if(!Owner) return;

	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Owner->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	const auto CurrentWeaponData = WeaponDataArray.FindByPredicate([&](const FWeaponData& Data) {
		//
		return Data.WeaponClass == CurrentWeapon->GetClass(); //
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Owner->GetMesh(), WeaponEquipSocketName);

	bEquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if(!Owner) return;

	Owner->PlayAnimMontage(Animation);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for(auto WeaponData : WeaponDataArray)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);
		if(!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}

		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if(!Owner || Owner->GetMesh() != MeshComponent) return;

	bEquipAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if(!Owner || Owner->GetMesh() != MeshComponent) return;

	bReloadAnimInProgress = false;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon
		&& !bEquipAnimInProgress
		&& !bReloadAnimInProgress
		&& CurrentWeapon->CanReload();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::OnEmpryClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if(!AmmoEmptyWeapon) return;

	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for(const auto Weapon : Weapons)
		{
			if(Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::ChangeClip()
{
	if(!CanReload()) return;

	StopFire();
	CurrentWeapon->ChangeClip();
	bReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if(CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetWeaponUIData();
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->IsAmmoFull();
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

void USTUWeaponComponent::Zoom(bool Enabled)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}

/////////////////////////////////////////////////////////////////////////////////
