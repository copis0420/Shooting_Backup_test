// Fill out your copyright notice in the Description page of Project Settings.


#include "test/TestEnemySpawner.h"
#include "Components/ArrowComponent.h"
#include "test/EnemyTest.h"

// Sets default values
ATestEnemySpawner::ATestEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime >= spawnTime)
	{
		GetWorld()->SpawnActor<AEnemyTest>(enemyFactory, Arrow->GetComponentLocation(), Arrow->GetComponentRotation());
		currentTime = 0;
	}

}


