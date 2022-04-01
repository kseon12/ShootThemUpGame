// I will find u. Copyrighted


#include "Menu/STUMenuHUD.h"

#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto MenuWidget =CreateWidget<USTUBaseWidget>(GetWorld(),MenuWidgetClass);
	if(MenuWidget)
	{
		MenuWidget->AddToViewport();
		MenuWidget->Show();
	}
}
