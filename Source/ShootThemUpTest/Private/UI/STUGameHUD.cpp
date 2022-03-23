// I will find u. Copyrighted


#include "UI/STUGameHUD.h"
#include "Blueprint/UserWidget.h"

 void ASTUGameHUD::BeginPlay() 
 {
     Super::BeginPlay();

     auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
     if (PlayerHUDWidget)
     {
         PlayerHUDWidget->AddToViewport();
     }
 }