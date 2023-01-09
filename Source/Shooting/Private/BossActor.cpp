// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyShootingGameModeBase.h"
#include "EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossActor::ABossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 300, 150));
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("Boss"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1, 6, 3));
}

// Called when the game starts or when spawned
void ABossActor::BeginPlay()
{
	Super::BeginPlay();

	startLocation = GetActorLocation();	
}

// Called every frame
void ABossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("BOSSACTORPATEERTN"));
	float patternDelay = 4.0f;
	if (currentTime > patternDelay)
	{
		BossAttack1(10, 20, 15);
		currentTime = 0;
	}
	else
	{
		currentTime += DeltaTime;
	}
	//현재 위치에서 (0, 0, 200) 위치까지 이동
	float distance =(startLocation - GetActorLocation()).Length();
	float shaking = FMath::RandRange(-1.0f, 1.0f);		

	//경과된 시간 누적
	if (distance < 500)	
	{
		FVector dir = GetActorUpVector() * -1;
		dir.Y = shaking;
		SetActorLocation(GetActorLocation() + dir * 200 * DeltaTime);
	}
	else
	{
		SetActorLocation(FVector(0, 0, 200));
	}	
	testValue += DeltaTime * 0.5f;
}


void ABossActor::BossAttack1(float angle, int32 count, float enemybulletSpace)
{
	//angle 간격으로 count 수만큼 총알을 생성
	//내가 한거 
		//for (int32 i = 0; i < count; i++)
		//{			
		//	float totalSize = (count - 1)*enemybulletSpace;
		//	float base_y = totalSize * -0.5f;
		//	float totalAngle = (count - 1) * angle;
		//	float Angle_X = totalAngle * -0.5F;
		//	FVector bulletLoc = FVector(0, base_y + angle* i, 0);
		//	FRotator bulletAng = FRotator(Angle_X+ angle * i-90.0f, 90.0f, 90.0f);
		//	GetWorld()->SpawnActor<AEnemyBullet>(bossBullet, bulletLoc, bulletAng);
		//}
	//1. 수학적 방법 & 회전의 두가지 방법
	//시작각도 = 270.0f - angle*(count-1)*0.5f ;
	float startAngle = 270 - angle * (count - 1) * 0.5f;
	for (int32 i = 0; i < count; i++)
	{
		//기본 좌표는 (0, rcos@, rsin@)
		FVector bulletBase = FVector(0, 50*FMath::Cos(FMath::DegreesToRadians(startAngle + angle * i)), 50*FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));
		
		//GetWorld()->SpawnActor<AEnemyBullet>(bossBullet, GetActorLocation() + bulletBase, FRotator(-90.0f, 0, startAngle + angle * i));
		AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bossBullet, GetActorLocation() + bulletBase, FRotator(-90, 0, 0));
		//enemyBullet->AddActorLocalRotation(FRotator(0, startAngle + angle * i, 0));
				
		//enemyBullet->SetDirection(bulletBase.GetSafeNormal());

		//쿼터니언 계산(x가 포워드 y과 라이트, z가 업) // 앞의 것은 고정하고 뒤의 것은 바꿀거야. 
		//Up Vector 축을 bulletBase방향으로 회전시켰을 때 로테이터 값을 계산. 
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(enemyBullet->GetActorUpVector(), bulletBase.GetSafeNormal());
		enemyBullet->SetActorRotation(rot);
		enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
	}
}