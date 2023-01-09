// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"
#include "MyShootingGameModeBase.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));
	//Collision Enabled ���� Query and Physics�� ����
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//���� ä���� Object Type�� "Enemy"�� ����
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//����ä���� �ϰ������� Ignore���·� ó��
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//����ä���� Player�� Bullet ä�ο� ���ؼ��� overlap���� ó��.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//��÷�� �ؼ� �ϳ��� ���� ����, �� �ٸ� �ϳ��� �÷��̾� �������� �̵�
	//1. ��÷. Ȯ���� ������ 7:3
	int32 drawNumber = FMath::RandRange(1, 100); //(1, 100)�� �ְ� ��ȯ�ڷ����� Ȯ���ϰ� ������ ����

	//2. ����, ���� ���� TraceRate���� ������
	if (drawNumber <= TraceRate)
	{
		//2-1. �÷��̾� ���͸� ã�´� 
		//AActor* Target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());
		
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		if (target != nullptr)
		{
			//float temp = target->moveSpeed;
			//2-2. �÷��̾� ��ġ - ���� ��ġ = �� ���� ����
			FVector TargetDir = target->GetActorLocation() - GetActorLocation();
			TargetDir.Normalize();
			direction = TargetDir;

			//target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);			
		}		
	}
	//3. �׷��� ������...
	else
	{
		direction = GetActorForwardVector();
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		//3-1. ������ ����
		if (target != nullptr)
		{
			
			//target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);			
		}
	}
	
	target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);
	target->forceMove.AddDynamic(this, &AEnemy::SetNewDirection);

	//�浹(������)�� �߻��ϸ� ������ �Լ��� ����
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	//������ �̺�Ʈ�� �߻��ϵ��� ����
	boxComp->SetGenerateOverlapEvents(true);

	//���� ��忡 enemies�迭�� �ڱ� �ڽ��� ����. (�����Ͷ� ��� ��Ī�Ѵٰ� ����.)
	AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gm != nullptr)
	{
		gm->enemies.Add(this);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����, �ε��� ����� �÷��̾��� 
	APlayerFlight* Playerflight = Cast<APlayerFlight>(OtherActor);

	if (Playerflight != nullptr)
	{
		//�÷��̾ �����ϰ� 
		Playerflight->Destroy();

		//�޴� ������ �����ϴ� �Լ��� ����
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if(gm != nullptr)
		{
			gm->ShowMenu();
		}

		Playerflight->ReservationHitColor(0.2f);

		//�� �ڽŵ� �����Ѵ� 
		Destroy();
	}
}

void AEnemy::DestroyMySelf()
{	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_FX, GetActorLocation(), GetActorRotation(), true);
	Destroy();
	AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
	AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
	myGM->AddScore(1);
}

void AEnemy::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	//�ڱ� �ڽ��� �迭���� ���� 
	//AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (gm != nullptr)
	//{
	//	gm->enemies.Remove(this);
	//}

	//��������Ʈ ����
	//target->playerBomb.RemoveDynamic(this, &AEnemy::DestroyMySelf());
}

void AEnemy::SetNewDirection(FVector newDir)
{
	//�̵� ������ newDir�� �ٲ۴�. 
	direction = newDir;
}

