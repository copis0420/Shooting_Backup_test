// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldActor.h"
#include "Components/Boxcomponent.h"
#include "Components/StaticMeshcomponent.h"
#include "Components/SceneComponent.h"
#include "PlayerFlight.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

// Sets default values
AShieldActor::AShieldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 150, 50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh Comp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector((0.5f, 1.5f, 0.5f)));
	
}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();
	
	//�÷��̾�� ���ʹ� ä���ϰ� �浹 ����(overlap)ó�� 
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//�浹�� ������ �Լ� ���ε� �ϱ�
	boxComp->OnComponentBeginOverlap.AddDynamic(this,&AShieldActor::ShieldOverlap);
}

// Called every frame
void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	//AddActorWorldRotation(FRotator(0, 0, 3));	//�̰Ŵ� 3���� ���ϴ°�
	//SetActorRotation(FRotator(0, 0, 3)) �̰Ŵ� 3���� �ٲٴ� ��. 
}

void AShieldActor::ShieldOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ε��� ����� ���ʹ̶��
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	//�ε��� ����� �÷��̾���
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	
	if (player != nullptr)
	{
		//�÷��̾�� �����ȴ�. 		
		//this->AttachToActor(player, FAttachmentTransformRules::KeepRelativeTransform);
		//this->AttachToActor(player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//this->AttachToActor(player, FAttachmentTransformRules::SnapToTargetIncludingScale);
		
		this->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform); 
		//player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform); //�θ� ���� �ڽ��� ����. 

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, false);
	}
	else if (enemy != nullptr)
	{
		//�浹�� ���ʹ̸� �ı��Ѵ�.
		enemy->Destroy();
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);

		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
		myGM->AddScore(1);
	}
}

void AShieldActor::ResetAttach()
{
	//�ٿ��� �θ� ���� 
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

