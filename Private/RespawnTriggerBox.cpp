#include "RespawnTriggerBox.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "SpartaCharacter.h"

void ARespawnTriggerBox::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &ARespawnTriggerBox::OnOverlap);
}

void ARespawnTriggerBox::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->IsA(ASpartaCharacter::StaticClass()))
    {
        // 캐릭터 가져오기
        ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(OtherActor);
        if (PlayerCharacter)
        {
            // 체력 10감소
            PlayerCharacter->DecreaseHealth(20.0f);

            // 월드에서 모든 PlayerStart 찾기
            TArray<AActor*> PlayerStarts;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

            if (PlayerStarts.Num() > 0)
            {
                FVector RespawnLocation = PlayerStarts[0]->GetActorLocation();
                PlayerCharacter->SetActorLocation(RespawnLocation);
            }
        }
    }
}
