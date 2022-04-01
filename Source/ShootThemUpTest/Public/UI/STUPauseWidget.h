// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

private:
	UFUNCTION()
	void OnClearPause();
};
