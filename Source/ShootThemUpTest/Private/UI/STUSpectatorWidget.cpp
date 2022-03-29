// I will find u. Copyrighted

#include "UI/STUSpectatorWidget.h"
#include "Components/STURespawnComponent.h"

/////////////////////////////////////////////////////////////////////////////////

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime)
{
	const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
	if(!RespawnComponent && !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
