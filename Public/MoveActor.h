#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveActor.generated.h"

UCLASS()
class SPARTAPROJECT_API AMoveActor : public AActor
{
    GENERATED_BODY()

public:
    AMoveActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    UStaticMeshComponent* StaticMeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float MoveSpeed = 200.0f; // 초당 이동 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float MoveDistance = 500.0f; // 왕복 이동 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float PauseTime = 1.5f; // 이동 후 멈추는 시간

private:
    FVector StartLocation;
    FVector EndLocation;
    bool bMovingForward = true;
    float PauseTimer = 0.0f;
    bool bPaused = false;
};