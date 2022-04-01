// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Show();
protected:
	UPROPERTY(meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* ShowAnimation;
};
