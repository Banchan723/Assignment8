#include "MoveActor.h"
#include "Math/UnrealMathUtility.h"

AMoveActor::AMoveActor()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 활성화

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(SceneRoot);
}

void AMoveActor::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();

    // 로컬 X축을 기준으로 이동 방향 설정
    FVector MoveOffset = GetActorForwardVector() * MoveDistance;
    EndLocation = StartLocation + MoveOffset;
}

void AMoveActor::Tick(float DeltaTime)
{
    if (bPaused)
    {
        PauseTimer -= DeltaTime;
        if (PauseTimer <= 0.0f)
        {
            bPaused = false;
        }
        return;
    }

    FVector TargetLocation = bMovingForward ? EndLocation : StartLocation;
    FVector CurrentLocation = GetActorLocation();

    // 로컬 축을 따라 이동 (프레임 독립적)
    FVector MoveDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector MoveOffset = MoveDirection * MoveSpeed * DeltaTime;

    FHitResult HitResult;
    AddActorWorldOffset(MoveOffset, true, &HitResult);

    if (FVector::Dist(CurrentLocation, TargetLocation) < 1.0f)
    {
        bMovingForward = !bMovingForward;
        bPaused = true;
        PauseTimer = PauseTime;
    }
}