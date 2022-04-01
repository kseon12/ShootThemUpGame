// I will find u. Copyrighted

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUWeaponComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

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

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
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

void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if(HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
	}
}

/////////////////////////////////////////////////////////////////////////////////
