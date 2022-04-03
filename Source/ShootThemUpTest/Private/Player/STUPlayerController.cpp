// I will find u. Copirighted

#include "Player/STUPlayerController.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

#include "Components/STURespawnComponent.h"


/////////////////////////////////////////////////////////////////////////////////

ASTUPlayerController::ASTUPlayerController()
{
	STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponents");
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnMatchStateChanged.AddUObject(this,&ASTUPlayerController::OnMatchStateChanged);
	}
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
	InputComponent->BindAction("Mute",IE_Pressed,this,&ASTUPlayerController::OnMuteSound);

}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::OnPauseGame()
{
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if(State == ESTUMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUPlayerController::OnMuteSound()
{
	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	STUGameInstance->ToggleVolume();
}

/////////////////////////////////////////////////////////////////////////////////
