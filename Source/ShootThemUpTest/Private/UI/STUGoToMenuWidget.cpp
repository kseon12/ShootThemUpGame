// I will find u. Copyrighted


#include "UI/STUGoToMenuWidget.h"
#include "STUGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

/////////////////////////////////////////////////////////////////////////////////

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUGoToMenuWidget::OnGoToMenu()
{
	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if(!STUGameInstance) return;

	UGameplayStatics::OpenLevel(this,STUGameInstance->GetMenuLevelName());
}

/////////////////////////////////////////////////////////////////////////////////
