// I will find u. Copyrighted


#include "STUGameInstance.h"
#include "STUSoundFunctionLibrary.h"

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
