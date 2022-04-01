// I will find u. Copyrighted


#include "Menu/STUMenuWidget.h"
#include "STUGameInstance.h"
#include "Menu/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}

	if(QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation != HideAnimation) return;

	const auto STUGameInstance = GetSTUGameInstance();
	if(!STUGameInstance) return;

	UGameplayStatics::OpenLevel(this,STUGameInstance->GetStartupLevel().LevelName);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	
}

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(),EQuitPreference::Quit,true);
}

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance();

	if(!STUGameInstance) return;

	checkf(STUGameInstance->GetLevelDataArray().Num()!=0, TEXT("LevelDataArray shoudn't be empty"));

	if(!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for (auto LevelData : STUGameInstance->GetLevelDataArray())
	{
		const auto LevelItemWidget= CreateWidget<USTULevelItemWidget>(GetWorld(),LevelItemWidgetClass);
		if(!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if(STUGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(STUGameInstance->GetLevelDataArray()[0]);
	}else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel());
	}
}

/////////////////////////////////////////////////////////////////////////////////

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance();

	STUGameInstance->SetStartupLevel(Data);

	for (auto LevelItemWidget: LevelItemWidgets)
	{
		if(LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance()
{
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}

/////////////////////////////////////////////////////////////////////////////////
