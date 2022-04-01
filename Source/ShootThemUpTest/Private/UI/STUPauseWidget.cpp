// I will find u. Copyrighted

#include "UI/STUPauseWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

/////////////////////////////////////////////////////////////////////////////////

bool USTUPauseWidget::Initialize()
{
	const auto InitStatus= Super::Initialize();

	if(ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}

	return InitStatus;
}

/////////////////////////////////////////////////////////////////////////////////

void USTUPauseWidget::OnClearPause()
{
	GetWorld()->GetAuthGameMode()->ClearPause();
}

/////////////////////////////////////////////////////////////////////////////////
