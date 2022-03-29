// I will find u. Copyrighted

#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

/////////////////////////////////////////////////////////////////////////////////

int32 USTUGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetSTUPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

/////////////////////////////////////////////////////////////////////////////////

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

/////////////////////////////////////////////////////////////////////////////////

int32 USTUGameDataWidget::GetTotalRoundNum() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetGameData().RoundNum : 0;
}

/////////////////////////////////////////////////////////////////////////////////

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

/////////////////////////////////////////////////////////////////////////////////

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
	return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

/////////////////////////////////////////////////////////////////////////////////

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
	return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}

/////////////////////////////////////////////////////////////////////////////////