// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTTHEMUPTEST_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	float PercentVisibilityThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	FLinearColor GoodColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI")
	FLinearColor BadColor = FLinearColor::Red;
};
