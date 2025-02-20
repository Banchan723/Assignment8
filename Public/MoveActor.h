#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveActor.generated.h"

UENUM(BlueprintType)
enum class EMoveAxis : uint8
{
    X UMETA(DisplayName = "X Axis"),
    Y UMETA(DisplayName = "Y Axis"),
    Z UMETA(DisplayName = "Z Axis")
};

UCLASS()
class SPARTAPROJECT_API AMoveActor : public AActor
{
    GENERATED_BODY()

public:
    AMoveActor();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    UStaticMeshComponent* StaticMeshComp;

    // 블루프린트에서 설정할 수 있도록 이동 속도와 이동 거리 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float MoveSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float MovementUpdateRate = 0.016f; // 이동 주기 (초 단위)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float MoveDistance = 500.0f; // 왕복 이동 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    EMoveAxis MoveAxis = EMoveAxis::X; // 이동할 축 (X 기본값)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    float PauseTime = 2.0f; // 이동 후 멈추는 시간 (0이면 멈추지 않음)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSettings")
    bool bReverseMovementAxis = true;  // 이동 축 반전

    FVector StartLocation;
    FVector EndLocation;
    bool bMovingForward = true;
    
    UPROPERTY()
    FTimerHandle MovementTimerHandle;

    UPROPERTY()
    FTimerHandle PauseTimerHandle;

    void MoveActor();
    void PauseMovement();
    void ResumeMovement();
    virtual void BeginPlay() override;
};