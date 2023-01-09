// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerBullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include "MyShootingGameModeBase.h"

// Sets default values
APlayerFlight::APlayerFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//�浹�ڽ� ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision")); //������� �߰� ����� ��

	//���� �� �浿�ڽ� ������Ʈ ��Ʈ ������Ʈ�� ����.
	SetRootComponent(boxComp);

	//�ڽ� �ݸ����� ũ�⸦ ����x����x���� ��� 50cm�� ����
	boxComp->SetBoxExtent(FVector(50, 50, 50));
	//boxComp->SetBoxExtent(FVector(50.0f)) 3���� ���̰� ������ �ϳ��� ��� ��. 

	//box collision�� �浹 ó�� �������� "PlayerPreset"���� �����Ѵ�. 
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset")); 

	//�޽� ������Ʈ ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//�޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ���
	meshComp->SetupAttachment(RootComponent); //RootComponent�� ���� ��Ʈ�� �ٲ� �������.
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*
	//�޽� ������Ʈ�� static mesh �׸� ť�� ���� �Ҵ�
	//"UObject/ConstructorHelpers.h" �߰��ؾ� ��. 
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	//�����н�(Copy Path)�� �ƴ� ���Ϸ��۷���(Copy File Reference) ���� 

	//ť�� ������ �ε��ϴµ� �����ϸ�
	if (cubeMesh.Succeeded())
	{
		//�ε��� ������ �޽� ������Ʈ�� static mesh �׸� ����.
		//->������ ����, .�� ���� ���� ���� �� Ŭ������ �ν��Ͻ��� ���� ����. 
		meshComp->SetStaticMesh(cubeMesh.Object);
	}
	*/
}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();

	//�÷��̾� ��Ʈ�ѷ� ĳ����
	APlayerController* playerCon = Cast<APlayerController>(GetController()); 
	
	//������ ���
	if (playerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}

	//���� ���� �� ����
	//my_mat = Cast<UMaterialInstanceDynamic>(meshComp->GetMaterial(0));
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo("myColor");

	//Material Interface���� ���� �Ķ���� ���� initColor ������ ����
	iMat->GetVectorParameterValue(param, initColor);
	UE_LOG(LogTemp, Warning, TEXT("R:%f, G:%f, B:%f"), initColor.R, initColor.G, initColor.B);
	//my_mat->GetVectorParameterValue(param, (FLinearColor)initColor);

	//Material Interface�� �̿��ؼ� Material Instance Dynamic ��ü ���� 
	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	//������ Dynamic Material�� mesh�� ���� 
	if(dynamicMat != nullptr)
	{
		meshComp->SetMaterial(0, dynamicMat);
	}
}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����� �Է¿� ���� ���� �̵�
	//������ ����ȭ 
	direction.Normalize(); 

	//����ڰ� �Է��� ������ �̵�
	// P = P0 +vt
	FVector dir = GetActorLocation() + direction * moveSpeed * DeltaTime;
	SetActorLocation(dir,true);
}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//������ UInputComponent* ������ UEhancedInputComponent*�� ��ȯ.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//�Լ�����
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	//�ν��� �Լ� ����
	enhancedInputComponent->BindAction(ia_booster, ETriggerEvent::Triggered, this, &APlayerFlight::StartBooster);
	enhancedInputComponent->BindAction(ia_booster, ETriggerEvent::Completed, this, &APlayerFlight::EndBooster);

	//ȭ�� �� �� ��� ���ִ� �Լ� ����
	enhancedInputComponent->BindAction(ia_special, ETriggerEvent::Triggered, this, &APlayerFlight::SpecialFire);
	//�� ���� �̵� �Լ� ����
	enhancedInputComponent->BindAction(ia_force, ETriggerEvent::Triggered, this, &APlayerFlight::ForceMove);

	
	////�Լ����� ���ε� ���ٰž�. 
	////Horizontal Axis �Է¿� �Լ� ����
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);

	////Vertical Axis �Է¿� �Լ� ����
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	////Fire Action �Է¿� �Լ� ���� 
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	//1. ������ Red�� ����
	dynamicMat->SetVectorParameterValue(("MyColor"), (FVector4)FLinearColor::Red); //���Ͼ��÷��� ���ͷ� ĳ��Ʈ 
	//2. ���� ������ �ǵ����� �Լ��� ���ε��� Ÿ�̸Ӹ� ���� 
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}


void APlayerFlight::ChangeOriginColor()
{
	dynamicMat->SetVectorParameterValue(("MyColor"), (FVector4)initColor); //���Ͼ��÷��� ���ͷ� ĳ��Ʈ 
	//my_mat->SetVectorParameterValue(TEXT("myColor"), initColor);
}

////�¿� �Է��� ���� �� ����� �Լ�
//void APlayerFlight::Horizontal(float value) 
//{
//	h = value;
//	UE_LOG(LogTemp, Warning, TEXT("h = %.2f"), h);
//	direction.Y = h;
//}
//
//���� �Է��� ������ �� ����� �Լ�
//void APlayerFlight::Vertical(float value)
//{
//	v = value;
//	UE_LOG(LogTemp, Warning, TEXT("v = %.2f"), v);
//	direction.Z = v;
//}


//���콺 ���� ��ư�� ������ �� ����� �Լ� 
void APlayerFlight::FireBullet()
{
	//�Ѿ��� �����Ѵ�.
	//�Ѿ� �������Ʈ ���� 
	/*
	int32 i = 1;
	for (i = 1; i <= bulletCount; i++ )
	{
		float f = i / 2; 
		FRotator spawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (f != 0)
		{
			FVector spawnPosition = GetActorLocation() + FVector(0, (i-1) * bulletSpacing, 0) + GetActorUpVector() * 90.0f;
			GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnPosition, spawnRotation, param);
		}
		else 
		{
			FVector spawnPosition = GetActorLocation() - FVector(0, -(i-1) * bulletSpacing, 0) + GetActorUpVector() * 90.0f;
			GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnPosition, spawnRotation, param);
		}
	}
	//FVector TempPosition = (GetActorLocation() + i + bulletSpacing) / i;
	*/
	//bulletCount��ŭ �Ѿ� �߻�
	if (!onTrap)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trap"));		
		return;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("noTrap"));
		for (int32 i = 0; i < bulletCount; i++)
		{
			//�Ѿ��� ��ü ����
			float totalSize = (bulletCount - 1) * bulletSpacing;

			//���� ��ġ
			float base_y = totalSize * -0.5f;

			//������ġ�� ������ ���͸� �����.
			FVector offset = FVector(0, base_y + bulletSpacing * i, 0);
			FVector spawnPosition = GetActorLocation() + GetActorForwardVector() * 90.0f;
			//�����¸�ŭ ��ġ�� ����
			spawnPosition += offset;

			FRotator spawnRotation = FRotator(90.0f, 0, 0);
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APlayerBullet* bullet = GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnPosition, spawnRotation, param);

			//������ �Ѿ�(Bullet)�� bulletAngle��ŭ �����ϰ� ȸ����Ų��. 
			//bullet->SetActorRotation(FRotator(90.0f, 0, bulletAngle));		
			//bullet->SetActorRelativeRotation(FRotator(90.0f, bulletAngle, 0));
			//bullet->AddActorLocalRotation(FRotator(0, 0, 45.0f));		
			/*
			float totalAngle = (bulletCount - 1) * bulletAngle;
			float base_angle = totalAngle*-0.5f;
			bullet->AddActorWorldRotation(FRotator(0, 0, base_angle + bulletAngle* i));
			*/
			float base_yaw = (bulletCount - 1) * bulletAngle * -0.5f;
			FRotator rot_base = FRotator(0, base_yaw + bulletAngle * i, 0);
			bullet->AddActorLocalRotation(rot_base);

		}

		//�Ѿ� �߻� ȿ���� ���� 
		UGameplayStatics::PlaySound2D(this, firesound);
	}
}

void APlayerFlight::Horizontal(const FInputActionValue& value)
{
	h = value.Get<float>();
	direction.Y= h;
}

void APlayerFlight::Vertical(const FInputActionValue& value)
{
	v = value.Get<float>();
	direction.Z = v;
}

void APlayerFlight::StartBooster()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Boost"));
	moveSpeed = moveSpeed * 2.0f;
}

void APlayerFlight::EndBooster()
{
	UE_LOG(LogTemp, Warning, TEXT("End Boost"));
	moveSpeed = moveSpeed / 4.0f;
}

void APlayerFlight::SpecialFire()
{	
	//��� enemy�� �ı��Ѵ�. 1�� ��� �������� �ٷ� ã�� ��
	//for (TActorIterator<AEnemy> it(GetWorld()); it; ++it) //TActorIterator�� <AEnemy>�� it(���� ����. �ƹ��ų� �ᵵ ��)�� GetWorld()���� ã�� ; ������Ʈ Ȯ���ϰ� ; enemy�� ������.
	//{
	//	AllEnemy = *it;
	//	AllEnemy->Destroy();
	//	FVector enemyLoc = AllEnemy->GetActorLocation();
	//	FRotator enemyRot = AllEnemy->GetActorRotation();
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);

	//	AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
	//	AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
	//	myGM->AddScore(1);
	//}
	// ��� enemy�� �ı��Ѵ� 2�� ���(�Լ��� Ȱ��_
	//for (TActorIterator<AEnemy> enemy(GetWorld()); enemy; ++enemy)
	//{
	//	enemy->DestroyMySelf();
	//}
	// 2. TArray<T> �迭�� �̿��� ���

	//AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());	
	//if (gm != nullptr)
	//{
	//	TArray<AEnemy*> enemyarray = gm->enemies;
	//	//for (int32 i = 0; i<gm->enemies.Num(); i++)
	//	for (int32 i = 0; i<enemyarray.Num(); i++)
	//	{
	//		//pending kill ���� üũ 
	//		if(IsValid(enemyarray[i]))
	//		{ 
	//			enemyarray[i]->DestroyMySelf();
	//		}
	//	}
	//	//����Ʈ�� �ʱ�ȭ 
	//	gm->enemies.Empty();
	//}

	//3. ��������Ʈ�� �����ϴ� ���
	playerBomb.Broadcast();
}

void APlayerFlight::ForceMove()
{
	forceMove.Broadcast(FVector(0, 1, 0));
}
