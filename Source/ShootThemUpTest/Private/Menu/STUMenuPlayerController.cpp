// I will find u. Copyrighted


#include "Menu/STUMenuPlayerController.h"
#include "STUGameInstance.h"

/////////////////////////////////////////////////////////////////////////////////

void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	
}

/////////////////////////////////////////////////////////////////////////////////