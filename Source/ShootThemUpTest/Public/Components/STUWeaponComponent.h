// I will find u. Copyrighted

#pragma once
#include "STUCoreTypes.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUPTEST_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUWeaponComponent();

	virtual void StartFire();
	virtual void StopFire();
	void NextWeapon();
	void Reload();

	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
	bool GetWeaponAmmoData(FAmmoData& AmmoDAta) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TArray<FWeaponData> WeaponDataArray;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	bool CanFire() const;
	bool CanEquip() const;

	void EquipWeapon(int32 WeaponIndex);
	int32 CurrentWeaponIndex = 0;

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool bEquipAnimInProgress = false;
	bool bReloadAnimInProgress = false;

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& Name);
	void SpawnWeapons();

	void PlayAnimMontage(UAnimMontage* Animation);

	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanReload() const;

	void OnEmpryClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};
