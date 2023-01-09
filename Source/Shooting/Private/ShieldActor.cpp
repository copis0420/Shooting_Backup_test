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
	
	//플레이어와 에너미 채널하고만 충돌 응답(overlap)처리 
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//충돌시 실행할 함수 바인딩 하기
	boxComp->OnComponentBeginOverlap.AddDynamic(this,&AShieldActor::ShieldOverlap);
}

// Called every frame
void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	//AddActorWorldRotation(FRotator(0, 0, 3));	//이거는 3도씩 더하는거
	//SetActorRotation(FRotator(0, 0, 3)) 이거는 3도로 바꾸는 것. 
}

void AShieldActor::ShieldOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//부딪힌 대상이 에너미라면
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	//부딪힌 대상이 플레이어라면
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	
	if (player != nullptr)
	{
		//플레이어에게 부착된다. 		
		//this->AttachToActor(player, FAttachmentTransformRules::KeepRelativeTransform);
		//this->AttachToActor(player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//this->AttachToActor(player, FAttachmentTransformRules::SnapToTargetIncludingScale);
		
		this->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform); 
		//player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform); //부모에 따라 자식이 돌아. 

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, false);
	}
	else if (enemy != nullptr)
	{
		//충돌한 에너미를 파괴한다.
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
	//붙였던 부모를 해제 
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

