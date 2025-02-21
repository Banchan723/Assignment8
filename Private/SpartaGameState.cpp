#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "Spartaplayercontroller.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 35.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay()
{
    Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
			ItemToSpawn = SpartaGameInstance->PersistentItemToSpawn;  // GameInstance에서 값 가져오기
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}


	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			SpartaGameInstance->PersistentItemToSpawn += 20;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("남은 시간 : %.1f"), RemainingTime)));
					TimeText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));

				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("점수 : %d"), SpartaGameInstance->TotalScore)));
						    ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));

						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d단계"), CurrentLevelIndex + 1)));
					LevelIndexText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
				}

				if (UTextBlock* TotalCoinText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TotalCoin"))))
				{
					TotalCoinText->SetText(FText::FromString(FString::Printf(TEXT("전체 코인% d / % d"), CollectedCoinCount, SpawnedCoinCount)));
					TotalCoinText->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
				}

				if (UTextBlock* ItemToSpawnText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ItemToSpawn"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())  // 현재 게임 인스턴스 가져오기
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);  // 캐스팅하여 사용
						if (SpartaGameInstance)
						{
							ItemToSpawnText->SetText(FText::FromString(FString::Printf(TEXT("레벨에 생성된 아이템 : %d"), SpartaGameInstance->PersistentItemToSpawn)));
							ItemToSpawnText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
						}
					}
				}

			}
		}
	}
}

void ASpartaGameState::LevelUpdateCoin()
{
}