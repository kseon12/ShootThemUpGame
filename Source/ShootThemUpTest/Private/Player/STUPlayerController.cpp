// I will find u. Copirighted

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"

/////////////////////////////////////////////////////////////////////////////////

ASTUPlayerController::ASTUPlayerController()
{
	STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponents");
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//OnNewPawn.Broadcast(InPawn);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);

}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::OnPauseGame()
{
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

/////////////////////////////////////////////////////////////////////////////////
