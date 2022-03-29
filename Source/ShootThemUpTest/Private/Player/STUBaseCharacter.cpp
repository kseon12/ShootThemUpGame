// I will find u. Copyrighted

#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

/////////////////////////////////////////////////////////////////////////////////

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

/////////////////////////////////////////////////////////////////////////////////

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////////////////////////

float ASTUBaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero()) return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& LinearColor)
{
	UE_LOG(LogBaseCharacter, Log, TEXT("Enter SetPlayerColor"));
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if(!MaterialInst)
	{
		UE_LOG(LogBaseCharacter, Log, TEXT("MatInst nullptr"));
		return;
	}

	MaterialInst->SetVectorParameterValue(MaterialColorName, LinearColor);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = GetCharacterMovement()->Velocity.Z;

	if(-FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, -FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUBaseCharacter::IsRunning() const
{
	return (bWantsToRun && bIsMovingForward);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::MoveForward(float Amount)
{
	bIsMovingForward = Amount > 0.01f;
	if(Amount == 0.0f) return;

	AddMovementInput(GetActorForwardVector(), Amount);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if(Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::StartRun()
{
	bWantsToRun = true;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUBaseCharacter::StopRun()
{
	bWantsToRun = false;
}

/////////////////////////////////////////////////////////////////////////////////
