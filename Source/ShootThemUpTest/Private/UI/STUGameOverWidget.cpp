// I will find u. Copyrighted

#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"

#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

/////////////////////////////////////////////////////////////////////////////////

void USTUGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
	}

	if(ResetLevelButton)
	{
		ResetLevelButton->OnClicked.AddDynamic(this,&USTUGameOverWidget::OnResetLevel);
	}
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
	if(State == ESTUMatchState::GameOver)
	{
		UpdatePlayersStat();
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUGameOverWidget::UpdatePlayersStat()
{
	if(!PlayerStatBox) return;

	PlayerStatBox->ClearChildren();

	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;

		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if(!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(FText::FromString(FString::FromInt(PlayerState->GetKillsNum())));
		PlayerStatRowWidget->SetDeaths(FText::FromString(FString::FromInt(PlayerState->GetDeathsNum())));
		PlayerStatRowWidget->SetTeam(FText::FromString(FString::FromInt(PlayerState->GetTeamID())));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
		PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

		PlayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUGameOverWidget::OnResetLevel()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}

/////////////////////////////////////////////////////////////////////////////////
