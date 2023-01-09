// Fill out your copyright notice in the Description page of Project Settings.


#include "TestStingAppender.h"
#include "PointerTest.h"

// Sets default values
ATestStingAppender::ATestStingAppender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestStingAppender::BeginPlay()
{
	Super::BeginPlay();

	NamePlus(A, B);	//문자열 합치기
	EvenChecker(Number);	//%를 활용한 이븐체커
	EvenCheckerMine(Number);	// /를 활용한 이븐체커
	MultiplicationTable(Number);	//구구단 함수	

	for (int32 i = 1; i < 10; i++) //구구단 1단부터 9단까지
	{
		for (int32 j = 1; j < 10; j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d x %d = %d"), i, j, i * j)
		}
	}

	for (int32 i = 0; i < ages.Num(); i++) //배열을 활용한 출력
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages[i]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages.Num());
	}

	/*
	for (auto Numeric : ages.Num()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages[i]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages.Num());
	}
		
	distances.Add("Seoul", 250.5f);
	distances.Add("Incheon", 450.5f);
	distances.Add("Sungnam", 130.5f);

	FString myKey = "Incheon";

	UE_LOG(LogTemp, Warning, TEXT("%s : %.2f"), *myKey, distances[myKey]);
	*/

	for (auto dist : distances) //맵에 있는 값(덩어리, pair)을 끝까지 하나씩 가져오는거야.
	{
		UE_LOG(LogTemp, Warning, TEXT("% s - %f"), *dist.Key, dist.Value);
	}

	/* 
	int32 number10 = 10;
	int32* copyNum = &number10;
	number10 = 30;

	UE_LOG(LogTemp, Warning, TEXT("%d, %d"), number10, *copyNum);

	for (APointerTest* pt : Pointertest)   배열 포인터 
	{
		Pointertest->pt_number = 15;
	}
	*/
	if (Pointertest != nullptr) //nullptr 체크. 포인터함수의 크래시를 막기 위한 보호장치
	{
		*(Pointertest->myAge) = 15;
		//Pointertest->realAge = 15;
	}
		

}

// Called every frame
void ATestStingAppender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestStingAppender::NamePlus(FString Name1, FString Name2) {
	FString Name = Name1 + " " + Name2;
	UE_LOG(LogTemp, Warning, TEXT("Your Name is %s"), *Name);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Name);
}

void ATestStingAppender::EvenChecker(int32 Abc)
{
	int32 Checker = Abc % 2;
	if (Checker == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EVEN"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UNEVEN"));
	}
}

void ATestStingAppender::EvenCheckerMine(float Abcd) 
	{
		float Checker2 = Abcd / 2;
		int32 Checker3 = Abcd / 2;
		float Checker4 = Checker2 - Checker3;
		if (Checker4 == 0) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Even"));
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Uneven"));
		}
	}

void ATestStingAppender::MultiplicationTable(int32 MT)
{
	for (int32 i = 1; i < 10; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d x %d = %d"), MT, i, i * MT)
	}

}


