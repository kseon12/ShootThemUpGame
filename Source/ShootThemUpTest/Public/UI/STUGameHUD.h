// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

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

	virtual void BeginPlay() override;

private:
	void OnMatchStateChanged(ESTUMatchState State);

	UPROPERTY()
	TMap<ESTUMatchState, UUserWidget*> GameWidgets;

	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

};
