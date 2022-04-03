// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"

#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASTUPlayerController();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTURespawnComponent* STURespawnComponent;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnMatchStateChanged(ESTUMatchState State);
	void OnMuteSound();
};
