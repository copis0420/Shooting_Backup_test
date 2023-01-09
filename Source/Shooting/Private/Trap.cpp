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
	//�浹 ������ ����
	sphComp->SetCollisionProfileName(TEXT("TrapPreset"));	
}


// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	//overlap ��������Ʈ�� InTrap �Լ� ���� 
	sphComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlap);
	sphComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OffOverlap);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���� �����ð�(currentTime)�� ������ �ð�(teleportTime)�� �����ߴٸ� 
	currentTime += DeltaTime;
	if (currentTime >= teleportTime)
	{
		//������ ��ġ(����)�� �����Ѵ�. 
		float drawNumber_X = FMath::RandRange(-500.0f, 500.0f);
		float drawNumber_Y = FMath::RandRange(-400.0f, 500.0f);
		/*FVector random_vec;
		random_vec.Y = drawNumber_X;
		random_vec.Z = drawNumber_Y;*/

		//���ֺ���. 3���� ���� �븻������ �� ���Ŀ� ����. �� ���·� ����. 
		//FVector random_vec = FMath::VRand()*500; ���Ϳ� ��Į�� ���ϸ� ũ�⸦ Ű��. 
		//random_vec.X = 0;  x�� ����. 


		//�� ��ġ�� �̵��Ѵ�. 
		SetActorLocation(FVector(0, drawNumber_Y, drawNumber_X));
		//�ð� �ʱ�ȭ
		currentTime = 0;
	}
	//�׷��� �ʴٸ�
	else
	{
		//�ð��� �����Ѵ�. 
		currentTime += DeltaTime;
	}
}


void ATrap::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���� �÷��̾��� ���...
	UE_LOG(LogTemp, Warning, TEXT("Warining2"));
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warining3"));
		//�÷��̾ �ִ� CanFire ������ false�� ���� 
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
	//���� ������ ������ �ε��� ���Ͱ� �÷��̾���
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
	//�÷��̾� canFire ������ ���� true�� ���� 
		player->onTrap = true;
	}

}
