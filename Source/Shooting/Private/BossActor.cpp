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
	//���� ��ġ���� (0, 0, 200) ��ġ���� �̵�
	float distance =(startLocation - GetActorLocation()).Length();
	float shaking = FMath::RandRange(-1.0f, 1.0f);		

	//����� �ð� ����
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
	//angle �������� count ����ŭ �Ѿ��� ����
	//���� �Ѱ� 
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
	//1. ������ ��� & ȸ���� �ΰ��� ���
	//���۰��� = 270.0f - angle*(count-1)*0.5f ;
	float startAngle = 270 - angle * (count - 1) * 0.5f;
	for (int32 i = 0; i < count; i++)
	{
		//�⺻ ��ǥ�� (0, rcos@, rsin@)
		FVector bulletBase = FVector(0, 50*FMath::Cos(FMath::DegreesToRadians(startAngle + angle * i)), 50*FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));
		
		//GetWorld()->SpawnActor<AEnemyBullet>(bossBullet, GetActorLocation() + bulletBase, FRotator(-90.0f, 0, startAngle + angle * i));
		AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bossBullet, GetActorLocation() + bulletBase, FRotator(-90, 0, 0));
		//enemyBullet->AddActorLocalRotation(FRotator(0, startAngle + angle * i, 0));
				
		//enemyBullet->SetDirection(bulletBase.GetSafeNormal());

		//���ʹϾ� ���(x�� ������ y�� ����Ʈ, z�� ��) // ���� ���� �����ϰ� ���� ���� �ٲܰž�. 
		//Up Vector ���� bulletBase�������� ȸ�������� �� �������� ���� ���. 
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(enemyBullet->GetActorUpVector(), bulletBase.GetSafeNormal());
		enemyBullet->SetActorRotation(rot);
		enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
	}
}