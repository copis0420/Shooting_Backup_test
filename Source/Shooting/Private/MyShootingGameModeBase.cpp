// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingGameModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BossActor.h"
#include "EnemySpawningPool.h"
#include "EngineUtils.h"


void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;
	// 만일 누적된 현재 점수가 최고점수도다 크다면
	if (currentScore > bestScore)
	{
		//최고 점수에 현재 점수를 넣는다. 
		bestScore = currentScore;	
		FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);
	}	
	
	//현재점수를 위젯의 curScore텍스트 블록에 반영. 
	if (main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}
	//FString testPath = FPaths::GetProjectFilePath(); //폴더 찾으려고 붙여 놓은 것들.
		//FString testPath = FPaths::EngineContentDir(); 엔진 설치 위치
		//FString testPath = FPaths::Combine("http://tonegray.com", "profile") 웹통신에서 자동적으로 '/'를 붙이며 합쳐줌. 
		//FString testPath = FPaths::ProjectContentDir() + TEXT("SaveScor/SaveScore.txt"); 주소와 추가 주소를 합쳐줌. 
		//UE_LOG(LogTemp, Warning, TEXT("Test path: %s"), *testPath);
		//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("Hello World"), true); 화면에 로그 출력하기 
	if (currentScore > 30 && !BisAppearBoss)
	{
		//AEnemySpawningPool* isBoss = Cast<AEnemySpawningPool>(isBoss);
				
		UE_LOG(LogTemp, Warning, TEXT("BOSS"));
		BisAppearBoss = true;
		FTimerHandle spawnHandle;
		GetWorld()->GetTimerManager().SetTimer(spawnHandle, this, &AMyShootingGameModeBase::SpawnBoss, 4, false);
		StopAllSpawn();		
	}
}

void AMyShootingGameModeBase::ShowMenu()
{
	menu_UI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);
	if (menuWidget != nullptr)
	{
		menu_UI->AddToViewport();
	}
	//화면을 일시 정지 상태로 만든다. 
	//1.직접 게임의 시간 흐름을 0으로 만든다.
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);

	//2.일시정지 함수를 호출한다. 
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//마우스 커서를 화면에 보이게 한다. 
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}	

void AMyShootingGameModeBase::SpawnBoss()
{
	UE_LOG(LogTemp, Warning, TEXT("Boss2222222222222"));
	GetWorld()->SpawnActor<ABossActor>(bossFactory, FVector(0, 0, 500), FRotator::ZeroRotator);
}

void AMyShootingGameModeBase::StopAllSpawn()
{
	for (TActorIterator<AEnemySpawningPool> pool(GetWorld()); pool; ++pool)
	{
		pool->isSpawn = false;
	}
}

void AMyShootingGameModeBase::BeginPlay() // : void AddScore(int32 count)
{
	//위젯 블루프린트 생성
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		//위젯을 뷰포트에 그림.
		main_UI->AddToViewport();

		//최고 점수를 파일에서 읽음.
		FString scoreText;
		FFileHelper::LoadFileToString(scoreText, *filePath);
		/*
		bool isLoadSuccess = FFileHelper::LoadFileToString(scoreText, *filePath);

		if (isLoadSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Success"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed"));
		}
		//	UE_LOG(LogTemp, Warning, TEXT("%s"), isLoadSuccess ? ("Success") : TEXT("Fail"));
*/	
		//읽어 온 값을 bestScore 변수에 넣음. 
		bestScore = FCString::Atoi(*scoreText);


		//현재 점수를 UI위젯의 curScore 텍스트를 블록에 반영 
		main_UI->PrintCurrentScore();

	}	
}
