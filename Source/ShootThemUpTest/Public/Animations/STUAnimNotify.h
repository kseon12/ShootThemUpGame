// I will find u. Copyrighted

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

/**
 * 
 */
UCLASS()
class SHOOTTHEMUPTEST_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    FOnNotifiedSignature OnNotified;
};
