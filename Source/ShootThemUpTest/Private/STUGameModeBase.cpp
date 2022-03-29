#include "STUGameModeBase.h"

#include "AIController.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "Player/STUPlayerState.h"
#include "Components/STURespawnComponent.h"

#include "AIController.h"
#include "EngineUtils.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All);

/////////////////////////////////////////////////////////////////////////////////

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ESTUMatchState::InProgress);
}

/////////////////////////////////////////////////////////////////////////////////

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

	if(KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}

	StartRespawn(VictimController);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnPlayer(Controller);
}

/////////////////////////////////////////////////////////////////////////////////

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto bPauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if(bPauseSet)
	{
		SetMatchState(ESTUMatchState::Pause);
	}

	return bPauseSet;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::SpawnBots()
{
	if(!GetWorld()) return;

	for(int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::StartRound()
{
	ResetPlayers();

	RoundCountDown = GameData.RoundTimeinSec;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::GameTimerUpdate()
{
	const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
	RoundCountDown -= TimerRate;

	if(FMath::IsNearlyEqual(RoundCountDown, 0, 0.1f) || RoundCountDown < 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if(CurrentRound + 1 <= GameData.RoundNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::ResetPlayers()
{
	if(!GetWorld()) return;

	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnPlayer(It->Get());
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::ResetOnPlayer(AController* Controller)
{
	if(Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::CreateTeamsInfo()
{
	if(!GetWorld()) return;

	int32 TeamID = 1;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;

		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		SetPlayerColor(Controller);
		TeamID = TeamID == 1 ? 2 : 1;
	}
}

/////////////////////////////////////////////////////////////////////////////////

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
	if(TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}

	return GameData.DefaultTeamColor;
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if(!Controller) return;

	const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if(!Character) return;

	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if(PlayerState)
	{
		UE_LOG(LogGameModeBase, Log, TEXT("%s"), *(PlayerState->GetTeamColor().ToString()));

		Character->SetPlayerColor(PlayerState->GetTeamColor());
	}
	else
	{
		Character->SetPlayerColor(GameData.DefaultTeamColor);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::LogPlayerInfo()
{
	if(!GetWorld()) return;

	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;

		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnComponent = Controller->FindComponentByClass<USTURespawnComponent>();
	if(!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTimeInSec);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::GameOver()
{
	LogPlayerInfo();

	for(auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if(Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMatchState(ESTUMatchState::GameOver);
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
	if(MatchState == State) return;

	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

/////////////////////////////////////////////////////////////////////////////////
