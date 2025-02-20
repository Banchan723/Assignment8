#include "MoveActor.h"
#include "Math/UnrealMathUtility.h"

AMoveActor::AMoveActor()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(SceneRoot);

    PrimaryActorTick.bCanEverTick = false;
}

void AMoveActor::BeginPlay()
{
    Super::BeginPlay();

    // 현재 월드에서 배치된 위치를 기준으로 시작 위치 설정
    StartLocation = GetActorLocation();

    // 이동 방향 설정
    FVector MoveOffset(0.0f, 0.0f, 0.0f);
    switch (MoveAxis)
    {
    case EMoveAxis::X:
        MoveOffset.X = MoveDistance;
        break;
    case EMoveAxis::Y:
        MoveOffset.Y = MoveDistance;
        break;
    case EMoveAxis::Z:
        MoveOffset.Z = MoveDistance;
        break;
    }

    // 블루프린트에서 설정한 `bReverseMovementAxis`에 따라 Start ↔ End 위치 반전
    if (bReverseMovementAxis)
    {
        EndLocation = StartLocation + MoveOffset;
    }
    else
    {
        EndLocation = StartLocation - MoveOffset;  // 🔥 반대 방향으로 이동 시작
    }

    // 첫 이동 방향 설정
    bMovingForward = true;  // 이렇게 하면 항상 Start → End로 이동함

    // 이동 타이머 시작
    ResumeMovement();
}



void AMoveActor::MoveActor()
{
    FVector TargetLocation = bMovingForward ? EndLocation : StartLocation;
    FVector CurrentLocation = GetActorLocation();

    // 이동 (선형 보간 사용)
    FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->DeltaTimeSeconds, MoveSpeed);
    SetActorLocation(NewLocation);

    // 목적지 도착 여부 확인
    if (FVector::Dist(CurrentLocation, TargetLocation) < 1.0f)
    {
        bMovingForward = !bMovingForward; // 방향 전환

        // 이동을 멈출지 여부 확인
        if (PauseTime > 0.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle); // 이동 타이머 제거
            GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &AMoveActor::ResumeMovement, PauseTime, false);
        }
    }
}

void AMoveActor::PauseMovement()
{
    GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
}

void AMoveActor::ResumeMovement()
{
    // 주기적으로 MoveActor()를 호출하여 이동 업데이트
    GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AMoveActor::MoveActor, MovementUpdateRate, true);
}