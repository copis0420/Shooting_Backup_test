// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestStingAppender.generated.h"

UCLASS()
class SHOOTING_API ATestStingAppender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestStingAppender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString A = "Park";
	UPROPERTY(EditAnywhere)
	FString B = "Il - seok";

	UFUNCTION(BlueprintCallable)
	void NamePlus(FString Name1, FString Name2);

	UPROPERTY(EditAnywhere)
	float Number = 3;
	void EvenChecker(int32 Abc);

	void EvenCheckerMine(float Abcd);

	void MultiplicationTable(int32 MT);
	
	UPROPERTY(EditAnywhere, Category = Variable)
	TArray<int32> ages = { 3, 4, 5, 6, 1555, 2777 };

	UPROPERTY(EditAnywhere, Category = Variable)
	TMap<FString, float> distances;

	UPROPERTY(EditAnywhere)
	//TArray <class APointerTest*> Pointertest;  //배열 포인터
	class APointerTest* Pointertest;  //헤더파일 내 포인터함수 전방선언

};
