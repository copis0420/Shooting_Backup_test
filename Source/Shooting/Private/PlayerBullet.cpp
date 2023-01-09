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
	
	//충돌 영역 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));
	

	//메쉬 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f)); 
}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();

	//충돌(오버랩)이 발생하면 실행할 함수를 연결
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);

	//오버랩 이벤트가 발생하도록 설정
	boxComp->SetGenerateOverlapEvents(true);

	//태어난 후 2초 뒤에 제거 
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
//	//만일, 부딪힌 대상이 총알이라면 
//	AEnemy* enemy = Cast<AEnemy>(OtherActor);
//
//	if (enemy != nullptr)
//	{
//		//적이 있던 위치에 폭발 이펙트를 생성
//		FVector enemyLoc = enemy->GetActorLocation();
//		FRotator enemyRot = enemy->GetActorRotation();
//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);
//
//
//		//게임 모드의 점수를 1점 추가한다. 
//		//AGameModeBase* gm = GetWorld()->GetGameMode();  2개는 같은 거야. 
//		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
//		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
//		myGM->AddScore(20);
//		UE_LOG(LogTemp, Warning, TEXT("Point:%d"), myGM->GetCurrentScore());
//		//총알을 제거하고
//		enemy->Destroy();
//
//		//나 자신도 제거한다 
//		Destroy();
//	}
//}

void APlayerBullet::DestroyMySelf()
{
	Destroy(); //따로 만드는 이유. 이펙트를 쓰고나 로고를 찍을 때는 따로만드는게 좋음. 
}