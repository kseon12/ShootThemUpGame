// I will find u. Copyrighted


#include "Menu/STUMenuGameModeBase.h"
#include "Menu/STUMenuPlayerController.h"
#include "Menu/STUMenuHUD.h"

/////////////////////////////////////////////////////////////////////////////////

ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
	PlayerControllerClass = ASTUMenuPlayerController::StaticClass();
	HUDClass = ASTUMenuHUD::StaticClass();
}

/////////////////////////////////////////////////////////////////////////////////
