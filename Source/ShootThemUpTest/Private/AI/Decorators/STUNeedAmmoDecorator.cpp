// I will find u. Copyrighted


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "Components/STUWeaponComponent.h"

#include "AIController.h"

/////////////////////////////////////////////////////////////////////////////////

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	NodeName = "NeedAmmo";
}

/////////////////////////////////////////////////////////////////////////////////

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(!Controller) return false;

	const auto WeaponComponent= Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
	if(!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);

	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}

/////////////////////////////////////////////////////////////////////////////////
