// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest.h"

// Sets default values
ACodeTest::ACodeTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodeTest::BeginPlay()
{
	Super::BeginPlay();
/*
	//만일 나이가 40살 이상이면 "꼰대"라고 출력.
	if (age >= 20 && age <= 40)
	{
		if (nation == "KOREA" || nation == "korea")
		{
			UE_LOG(LogTemp, Warning, TEXT("You are a man"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You are not my man"));			
		}
	}
	else if(age > 40)
	{
		UE_LOG(LogTemp, Warning, TEXT("You are too old man"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You are too young man"));
	}
	

	if (nation == "KOREA" && age >= 20 && age < 40)
	{
		UE_LOG(LogTemp, Warning, TEXT("Young Man"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Old Man"));
	}
	*/
	if (nation == "KOREA" && "korea")
	{
		if (age >= 20 && age < 40)
		{
			UE_LOG(LogTemp, Warning, TEXT("My Young Man"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Just My Man"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not My Man"));
	}
}

// Called every frame
void ACodeTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

