// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

	virtual void BeginPlay() override;
};
