// I will find u. Copyrighted

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

/////////////////////////////////////////////////////////////////////////////////

void USTUAIWeaponComponent::StartFire()
{
	if(!CanFire()) return;

	if(CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		CurrentWeapon->StartFire();
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUAIWeaponComponent::StopFire()
{
	if(!CanFire()) return;

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while(NextIndex != CurrentWeaponIndex)
	{
		if(!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if(CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////////
