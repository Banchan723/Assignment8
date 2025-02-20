#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SpartaCharacter.h"
#include "RespawnTriggerBox.generated.h"

UCLASS()
class SPARTAPROJECT_API ARespawnTriggerBox : public ATriggerBox
{
    GENERATED_BODY()

protected:


    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
