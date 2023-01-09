// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawningPool.h"
#include "Enemy.h"
#include "Components/ArrowComponent.h"

// Sets default values
AEnemySpawningPool::AEnemySpawningPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(rootScene);

	spawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Arrow"));
	spawnArrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawningPool::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AEnemySpawningPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isSpawn)
	{ 	
		return;
	}
		//����� �ð� ����
		currentTime += DeltaTime;

	//����, ������ �ð��� spawnCool�� ������ �ð����� ũ�ų� ���ٸ� 
	if (currentTime >= spawnCool)
	{
		//EnemyBlueprint ����
		//GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation());
		//Arrow���⿡ ���� ���� 
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnArrow->GetComponentLocation(), spawnArrow->GetComponentRotation());

		//������ �ð��� 0�ʷ� �ʱ�ȭ 
		currentTime = 0;
	}	
}

