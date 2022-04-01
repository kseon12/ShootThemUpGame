// I will find u. Copyrighted

#pragma once

#include "STUCoreTypes.h"
#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
UCLASS()
class SHOOTTHEMUPTEST_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetWeaponUIdata(FWeaponUIData& WeaponUIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString FormatBullets(int32 BulletNum) const;

	virtual bool Initialize() override;

protected:
	UPROPERTY(meta= (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta= (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	FLinearColor GoodColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	FLinearColor BadColor = FLinearColor::Red;

private:
	void OnHealthChanged(float Health, float HealthDelta);
	void OnNewPawn(APawn* Pawn);

	void UpdateHealthBar();
};
