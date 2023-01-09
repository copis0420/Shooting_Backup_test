// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooting1.h"

// Sets default values
AShooting1::AShooting1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooting1::BeginPlay()
{
	Super::BeginPlay();
	
	// = 는 넣는다. == 는 같다의 개념. 

	number = 2;
	number2 = number;
	number2 = -7;
	UE_LOG(LogTemp, Warning, TEXT("Number is %d, Number2 is %d"), number, number2);
	UE_LOG(LogTemp, Warning, TEXT("HelloWorld"));

	fnumber1 = 3.141592f;
	fnumber2 = fnumber1 / number;

	UE_LOG(LogTemp, Warning, TEXT("Float Number1 is %f, Float Number2 is %.5f"), fnumber1, fnumber2);
	UE_LOG(LogTemp, Warning, TEXT("박원석"));
	//UE_LOG(LogTemp, Warning, TEXT("FString is %s"), *TEXT(name2)); 유니코드는 TEXT내에 있는 것만 되는데 문자열은 TEXT안에 없어서 문제가 있음. 
	//문자열은 char를 포인터처럼 받아와야 해서 *를 붙임. 문자열 안의 값을 꺼내오라는 뜻.
	UE_LOG(LogTemp, Warning, TEXT("박원석은 유니파이로 입력을 해야 하지만 변수로 하려면 TEXT로 감싸야 이렇게 %s 출력돼"), *name2);

	isTeacher = false;

	if (isTeacher == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("그는 선생님이다."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("그는 학생이다."));
	}

	//출력 방법. output or screen

	int32 result = Add(9, 10); //지역변수
	UE_LOG(LogTemp, Warning, TEXT("더하기 = %d"), result);
	int32 result1 = Minus(9, 10); 
	UE_LOG(LogTemp, Warning, TEXT("빼기 = %d"), result1);
	int32 result2 = Multiply(9, 10); 
	UE_LOG(LogTemp, Warning, TEXT("곱하기 = %d"), result2);
	float result3 = Divide(5, 2);  //반환자료형을 설정해줘야함. 
	UE_LOG(LogTemp, Warning, TEXT("나누기 = %.2f"), result3);


}

// Called every frame
void AShooting1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


	int32 AShooting1::Add(INT Number1, INT Number2) {
		return Number1 + Number2;
	}
	int32 AShooting1::Minus(INT Number1, INT Number2) {
		return Number1 - Number2;
	}
	int32 AShooting1::Multiply(INT Number1, INT Number2) {
		return Number1 * Number2;
	}
	float AShooting1::Divide(int32 Number1, int32 Number2) {
		return (float)Number1 / (float)Number2; //정수로 받지만 (float)를 넣음으로 실수로 바꿈. 숫자형에서만 가능한 것. 
	}
