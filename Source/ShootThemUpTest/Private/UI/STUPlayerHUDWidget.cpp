// I will find u. Copyrighted

#include "UI/STUPlayerHUDWidget.h"
#include "STUUtils.h"

#include "Components/ProgressBar.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

/////////////////////////////////////////////////////////////////////////////////

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUPlayerHUDWidget::GetWeaponUIdata(FWeaponUIData& WeaponUIData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());;
	if(!WeaponComponent) return false;

	return WeaponComponent->GetWeaponUIData(WeaponUIData);
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if(!WeaponComponent) return false;

	return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

	return HealthComponent && !HealthComponent->IsDead();
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto PlayerController = GetOwningPlayer();

	return PlayerController && (PlayerController->GetStateName() == NAME_Spectating);
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUPlayerHUDWidget::Initialize()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn()); 
	}

	return Super::Initialize();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if(HealthDelta < 0)
	{
		OnTakeDamage();
		if(!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUPlayerHUDWidget::OnNewPawn(APawn* Pawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Pawn);
	if(HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
	}

	UpdateHealthBar();
}

/////////////////////////////////////////////////////////////////////////////////

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
	const auto Controller = GetOwningPlayer();
	if(!Controller) return 0;
	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	return PlayerState ? PlayerState->GetKillsNum() : 0; 
}

/////////////////////////////////////////////////////////////////////////////////

FString USTUPlayerHUDWidget::FormatBullets(int32 BulletNum) const
{
	const int32 MaxLen=3;
	const TCHAR PrefixSymbol = '0';

	auto BulletStr = FString::FromInt(BulletNum);
	const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();

	if(SymbolsNumToAdd>0)
	{
		BulletStr= FString::ChrN(SymbolsNumToAdd,PrefixSymbol).Append(BulletStr);
	}

	return BulletStr;
}

/////////////////////////////////////////////////////////////////////////////////

void USTUPlayerHUDWidget::UpdateHealthBar()
{
	if(HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent()> PercentColorThreshold? GoodColor : BadColor);
	}
}

/////////////////////////////////////////////////////////////////////////////////
