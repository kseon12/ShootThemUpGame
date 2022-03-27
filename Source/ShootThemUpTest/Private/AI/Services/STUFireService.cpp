// I will find u. Copyrighted

#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "Components/STUAIPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Controller = OwnerComp.GetAIOwner();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if(Controller)
	{
		const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
		if(WeaponComponent)
		{
			HasAim ? WeaponComponent->StartFire() :WeaponComponent->StopFire();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
