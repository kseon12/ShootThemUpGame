// I will find u. Copyrighted

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUWeaponComponent.h"

#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/////////////////////////////////////////////////////////////////////////////////

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto STUController = Cast<AAIController>(Controller);
	if(STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
	}
}

/////////////////////////////////////////////////////////////////////////////////
