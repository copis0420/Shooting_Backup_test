// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"


// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//�浹 ���� ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));
	

	//�޽� ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f)); 
}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();

	//�浹(������)�� �߻��ϸ� ������ �Լ��� ����
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);

	//������ �̺�Ʈ�� �߻��ϵ��� ����
	boxComp->SetGenerateOverlapEvents(true);

	//�¾ �� 2�� �ڿ� ���� 
	//SetLifeSpan(2.0f);

	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &APlayerBullet::DestroyMySelf, 2.0f, false);

}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//direction = FVector(1, 0, 0);
	direction = GetActorForwardVector();

	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
}

void APlayerBullet::SetDirection(FVector value)
{
	direction = value;
}

//void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//����, �ε��� ����� �Ѿ��̶�� 
//	AEnemy* enemy = Cast<AEnemy>(OtherActor);
//
//	if (enemy != nullptr)
//	{
//		//���� �ִ� ��ġ�� ���� ����Ʈ�� ����
//		FVector enemyLoc = enemy->GetActorLocation();
//		FRotator enemyRot = enemy->GetActorRotation();
//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);
//
//
//		//���� ����� ������ 1�� �߰��Ѵ�. 
//		//AGameModeBase* gm = GetWorld()->GetGameMode();  2���� ���� �ž�. 
//		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
//		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
//		myGM->AddScore(20);
//		UE_LOG(LogTemp, Warning, TEXT("Point:%d"), myGM->GetCurrentScore());
//		//�Ѿ��� �����ϰ�
//		enemy->Destroy();
//
//		//�� �ڽŵ� �����Ѵ� 
//		Destroy();
//	}
//}

void APlayerBullet::DestroyMySelf()
{
	Destroy(); //���� ����� ����. ����Ʈ�� ���� �ΰ� ���� ���� ���θ���°� ����. 
}