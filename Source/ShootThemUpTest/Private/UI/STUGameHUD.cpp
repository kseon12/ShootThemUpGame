// I will find u. Copyrighted

#include "UI/STUGameHUD.h"
#include"STUGameModeBase.h"

#include "UI/STUBaseWidget.h"

/////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTUMatchState::InProgress,CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::Pause,CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESTUMatchState::GameOver,CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

	for(auto GameWidgetPair : GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if(GameWidget)
		{
			GameWidget->AddToViewport();
			GameWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}


	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnMatchStateChanged.AddUObject(this,&ASTUGameHUD::OnMatchStateChanged);
	}

	
}

/////////////////////////////////////////////////////////////////////////////////

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}
	UE_LOG(LogSTUGameHUD,Display,TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

/////////////////////////////////////////////////////////////////////////////////
