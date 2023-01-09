// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerFlight.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphComp);
	sphComp->SetSphereRadius(200.0f);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(4.0f));
	//충돌 프리셋 설정
	sphComp->SetCollisionProfileName(TEXT("TrapPreset"));	
}


// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	//overlap 델리게이트에 InTrap 함수 연결 
	sphComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlap);
	sphComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OffOverlap);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//현재 누적시간(currentTime)이 지정된 시간(teleportTime)에 도달했다면 
	currentTime += DeltaTime;
	if (currentTime >= teleportTime)
	{
		//랜덤한 위치(벡터)를 설정한다. 
		float drawNumber_X = FMath::RandRange(-500.0f, 500.0f);
		float drawNumber_Y = FMath::RandRange(-400.0f, 500.0f);
		/*FVector random_vec;
		random_vec.Y = drawNumber_X;
		random_vec.Z = drawNumber_Y;*/

		//유닛벡터. 3개의 값이 노말라이즈 된 이후에 나옴. 구 형태로 나와. 
		//FVector random_vec = FMath::VRand()*500; 벡터에 스칼라를 곱하면 크기를 키워. 
		//random_vec.X = 0;  x축 고정. 


		//그 위치로 이동한다. 
		SetActorLocation(FVector(0, drawNumber_Y, drawNumber_X));
		//시간 초기화
		currentTime = 0;
	}
	//그렇지 않다면
	else
	{
		//시간을 누적한다. 
		currentTime += DeltaTime;
	}
}


void ATrap::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약 플레이어인 경우...
	UE_LOG(LogTemp, Warning, TEXT("Warining2"));
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warining3"));
		//플레이어에 있는 CanFire 변수를 false로 변경 
		//player->GetComponents(onTrap);
		player->onTrap = false;
		//APlayerFlight::FireBullet()
		//APlayerFlight::FireBullet()->SetActive(false)
	}
	//else
	//{
	//	player->onTrap = true;
	//}
}

void ATrap::OffOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//만일 나가기 직전에 부딪힌 액터가 플레이어라면
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
	//플레이어 canFire 변수의 값을 true로 변경 
		player->onTrap = true;
	}

}
