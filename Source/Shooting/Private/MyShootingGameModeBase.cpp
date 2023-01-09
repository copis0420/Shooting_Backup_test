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
	// ���� ������ ���� ������ �ְ��������� ũ�ٸ�
	if (currentScore > bestScore)
	{
		//�ְ� ������ ���� ������ �ִ´�. 
		bestScore = currentScore;	
		FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);
	}	
	
	//���������� ������ curScore�ؽ�Ʈ ��Ͽ� �ݿ�. 
	if (main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}
	//FString testPath = FPaths::GetProjectFilePath(); //���� ã������ �ٿ� ���� �͵�.
		//FString testPath = FPaths::EngineContentDir(); ���� ��ġ ��ġ
		//FString testPath = FPaths::Combine("http://tonegray.com", "profile") ����ſ��� �ڵ������� '/'�� ���̸� ������. 
		//FString testPath = FPaths::ProjectContentDir() + TEXT("SaveScor/SaveScore.txt"); �ּҿ� �߰� �ּҸ� ������. 
		//UE_LOG(LogTemp, Warning, TEXT("Test path: %s"), *testPath);
		//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("Hello World"), true); ȭ�鿡 �α� ����ϱ� 
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
	//ȭ���� �Ͻ� ���� ���·� �����. 
	//1.���� ������ �ð� �帧�� 0���� �����.
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);

	//2.�Ͻ����� �Լ��� ȣ���Ѵ�. 
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//���콺 Ŀ���� ȭ�鿡 ���̰� �Ѵ�. 
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
	//���� �������Ʈ ����
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		//������ ����Ʈ�� �׸�.
		main_UI->AddToViewport();

		//�ְ� ������ ���Ͽ��� ����.
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
		//�о� �� ���� bestScore ������ ����. 
		bestScore = FCString::Atoi(*scoreText);


		//���� ������ UI������ curScore �ؽ�Ʈ�� ��Ͽ� �ݿ� 
		main_UI->PrintCurrentScore();

	}	
}
