#pragma once

class AActor;

class STUUtils
{
public:
	template<typename T>
	static T* GetSTUPlayerComponent(AActor* SomeActor)
	{
		if(!SomeActor) return nullptr;

		const auto Component = SomeActor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
