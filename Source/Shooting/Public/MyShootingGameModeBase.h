// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMenuWidget> menuWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class ABossActor> bossFactory;

	TArray<class AEnemy*> enemies; 	

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() { return currentScore; } //포스인라인은 1줄, CPP에 함수 따로 안 만들어도 됨
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetBestScore() { return bestScore; } 
	

	//virtual이 붙은 것은 추상 클래스. 중괄호 안에 아무 것도 안 만들거나 상속해서 덮어 쓸 수 있게 만드는 것. 
	virtual void BeginPlay() override;

	void AddScore(int32 count);

	void ShowMenu();

private:
	int32 currentScore = 0;
	int32 bestScore = 0;
	class UMainWidget* main_UI;
	class UMenuWidget* menu_UI;

	//최고 점수를 파일로 저장 
	FString filePath = FString("D:/UR/Shooting/Contents/SaveScore/BestScore.TXT"); //절대 경로
	//FString filePath2 = FString("...\Shooting\SaveScore"); //상대 경로
	bool BisAppearBoss = false;

	void SpawnBoss();
	void StopAllSpawn();	
};
