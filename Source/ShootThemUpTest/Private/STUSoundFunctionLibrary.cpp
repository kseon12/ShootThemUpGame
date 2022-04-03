// I will find u. Copyrighted


#include "STUSoundFunctionLibrary.h"

#include "Sound/SoundClass.h"

void USTUSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if(!SoundClass) return;
	SoundClass->Properties.Volume = FMath::Clamp(Volume,0.0f,1.0f);
}

void USTUSoundFunctionLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if(!SoundClass) return;

	const auto NextVolume = SoundClass->Properties.Volume> 0.0f ? 0.0f : 1.0f;
	SetSoundClassVolume(SoundClass,NextVolume);
}
