// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API ASTUMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
