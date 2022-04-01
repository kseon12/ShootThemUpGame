// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SHOOTTHEMUPTEST_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnLevelSelectedSignature OnLevelSelected;

	void SetLevelData(const FLevelData& Data);
	const FLevelData& GetLevelData(){return LevelData;}

	void SetSelected(bool IsSelected);
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelSelectedButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* FrameImage;

	virtual void NativeOnInitialized() override;

private:
	FLevelData LevelData;

	UFUNCTION()
	void OnLevelItemClicked();

	UFUNCTION()
	void OnLevelItemHovered();

	UFUNCTION()
	void OnLevelItemUnhovered();
};
