#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

UCLASS()
class SPARTAPROJECT_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USpartaGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 PersistentItemToSpawn;

	UFUNCTION(Blueprintcallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
