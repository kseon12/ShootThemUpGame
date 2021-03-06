// I will find u. Copyrighted

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

/////////////////////////////////////////////////////////////////////////////////

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if(STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const auto AimActor = STUAIPerceptionComponent->GetClosestEnemy();
	SetFocus(AimActor);
}

/////////////////////////////////////////////////////////////////////////////////

AActor* ASTUAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) return nullptr;

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

/////////////////////////////////////////////////////////////////////////////////
