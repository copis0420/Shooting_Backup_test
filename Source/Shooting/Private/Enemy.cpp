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
	//Collision Enabled 값을 Query and Physics로 설정
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//응답 채널의 Object Type을 "Enemy"로 설정
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//응답채널을 일괄적으로 Ignore상태로 처리
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//응답채널의 Player와 Bullet 채널에 대해서만 overlap으로 처리.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//추첨을 해서 하나는 정면 방향, 또 다른 하나는 플레이어 방향으로 이동
	//1. 추첨. 확률은 변수로 7:3
	int32 drawNumber = FMath::RandRange(1, 100); //(1, 100)을 넣고 반환자료형을 확인하고 정수형 선언

	//2. 만약, 뽑은 값이 TraceRate보다 작으면
	if (drawNumber <= TraceRate)
	{
		//2-1. 플레이어 액터를 찾는다 
		//AActor* Target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());
		
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		if (target != nullptr)
		{
			//float temp = target->moveSpeed;
			//2-2. 플레이어 위치 - 나의 위치 = 갈 방향 설정
			FVector TargetDir = target->GetActorLocation() - GetActorLocation();
			TargetDir.Normalize();
			direction = TargetDir;

			//target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);			
		}		
	}
	//3. 그렇지 않으면...
	else
	{
		direction = GetActorForwardVector();
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		//3-1. 방향은 정면
		if (target != nullptr)
		{
			
			//target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);			
		}
	}
	
	target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);
	target->forceMove.AddDynamic(this, &AEnemy::SetNewDirection);

	//충돌(오버랩)이 발생하면 실행할 함수를 연결
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	//오버랩 이벤트가 발생하도록 설정
	boxComp->SetGenerateOverlapEvents(true);

	//게임 모드에 enemies배열에 자기 자신을 넣음. (포인터라서 사실 지칭한다가 맞음.)
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
	//만일, 부딪힌 대상이 플레이어라면 
	APlayerFlight* Playerflight = Cast<APlayerFlight>(OtherActor);

	if (Playerflight != nullptr)
	{
		//플레이어를 제거하고 
		Playerflight->Destroy();

		//메뉴 위젯을 생성하는 함수를 실행
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if(gm != nullptr)
		{
			gm->ShowMenu();
		}

		Playerflight->ReservationHitColor(0.2f);

		//나 자신도 제거한다 
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
	//자기 자신을 배열에서 제거 
	//AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (gm != nullptr)
	//{
	//	gm->enemies.Remove(this);
	//}

	//델리게이트 삭제
	//target->playerBomb.RemoveDynamic(this, &AEnemy::DestroyMySelf());
}

void AEnemy::SetNewDirection(FVector newDir)
{
	//이동 방향을 newDir로 바꾼다. 
	direction = newDir;
}

