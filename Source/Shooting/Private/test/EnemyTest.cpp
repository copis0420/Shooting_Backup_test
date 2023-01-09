// Fill out your copyright notice in the Description page of Project Settings.


#include "test/EnemyTest.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemyTest::AEnemyTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(60.0f));

	mehsComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mehsComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyTest::BeginPlay()
{
	Super::BeginPlay();
	direction = GetActorForwardVector();
}

// Called every frame
void AEnemyTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + moveSpeed * direction * DeltaTime);
}

