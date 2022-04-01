// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

class USTUBaseWidget;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

public:
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	void OnMatchStateChanged(ESTUMatchState State);

	UPROPERTY()
	TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;

	UPROPERTY()
	USTUBaseWidget* CurrentWidget = nullptr;

};
