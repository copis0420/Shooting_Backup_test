// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shooting1.generated.h"

UCLASS()
class SHOOTING_API AShooting1 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShooting1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//UPROPERTY(EditAnywhere)  //블루프린트에서 수정가능하게 선언. 바로 밑에 있는 변수에만 적용. 이런 것을 리플렉션 기능이라고 함. 
	//UPROPERTY(EditDefaultsOnly)  //디폴트에서만 수정하게 설정
	//UPROPERTY(EditInstanceOnly)  //인스턴스에서만 수정하게 설정
	//UPROPERTY(EditAnywhere, BlueprintReadWrite) //읽고 쓰게 할 수 있게 설정
	//UPROPERTY(EditAnywhere, BlueprintReadOnly) // 읽기만 할 수 있게 설정
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CodeVarible) //CodeVarible이란 카테고리내에 생성
	//UPROPERTY(VisibleAnywhere)  //어디서나 볼 수 있게 설정


	//정수형 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	int32 number;
	int32 number2;

	//실수형 변수
	float fnumber1;
	float fnumber2 = 0; //선언과 동시에 초기화
	float fnumber3 = 10;

	//문자열변수
	FString name = "";
	FString name2 = FString(TEXT("박원석"));
	//TEXT 혹은 L로 할 수 있어 

	////논리형변수 
	bool isStudent;
	bool isStudent1 = false;
	bool isTeacher = true; //선언과 동시에 초기화

	int32 Add(int32 Number1, int32 Number2);
	int32 Minus(int32 Number1, int32 Number2);
	int32 Multiply(int32 Number1, int32 Number2);
	float Divide(int32 Number1, int32 Number2);
};