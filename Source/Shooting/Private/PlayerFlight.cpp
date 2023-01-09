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
	
	//충돌박스 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision")); //헤더파일 추가 해줘야 함

	//생성 된 충동박스 컴포넌트 루트 컴포넌트로 설정.
	SetRootComponent(boxComp);

	//박스 콜리전의 크기를 가로x세롷x높이 모두 50cm로 설정
	boxComp->SetBoxExtent(FVector(50, 50, 50));
	//boxComp->SetBoxExtent(FVector(50.0f)) 3변의 길이가 같으면 하나만 적어도 됨. 

	//box collision에 충돌 처리 프리셋을 "PlayerPreset"으로 설정한다. 
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset")); 

	//메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록
	meshComp->SetupAttachment(RootComponent); //RootComponent라 스면 루트가 바뀌어도 상관없음.
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*
	//메시 컴포넌트의 static mesh 항목에 큐브 파일 할당
	//"UObject/ConstructorHelpers.h" 추가해야 함. 
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	//파일패스(Copy Path)가 아닌 파일레퍼런스(Copy File Reference) 복사 

	//큐브 파일을 로드하는데 성공하면
	if (cubeMesh.Succeeded())
	{
		//로드한 파일을 메시 컴포넌트의 static mesh 항목에 넣음.
		//->포인터 변수, .을 찍은 것은 직접 그 클래스로 인스턴스를 만든 변수. 
		meshComp->SetStaticMesh(cubeMesh.Object);
	}
	*/
}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 컨트롤러 캐스팅
	APlayerController* playerCon = Cast<APlayerController>(GetController()); 
	
	//포인터 방어
	if (playerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}

	//현재 색상 값 저장
	//my_mat = Cast<UMaterialInstanceDynamic>(meshComp->GetMaterial(0));
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo("myColor");

	//Material Interface에서 벡터 파라미터 값을 initColor 변수에 저장
	iMat->GetVectorParameterValue(param, initColor);
	UE_LOG(LogTemp, Warning, TEXT("R:%f, G:%f, B:%f"), initColor.R, initColor.G, initColor.B);
	//my_mat->GetVectorParameterValue(param, (FLinearColor)initColor);

	//Material Interface를 이용해서 Material Instance Dynamic 개체 생성 
	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	//생성한 Dynamic Material을 mesh에 설정 
	if(dynamicMat != nullptr)
	{
		meshComp->SetMaterial(0, dynamicMat);
	}
}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//사용자 입력에 따른 방향 이동
	//벡터의 정규화 
	direction.Normalize(); 

	//사용자가 입력한 방향대로 이동
	// P = P0 +vt
	FVector dir = GetActorLocation() + direction * moveSpeed * DeltaTime;
	SetActorLocation(dir,true);
}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//기존의 UInputComponent* 변수를 UEhancedInputComponent*로 변환.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//함수연결
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	//부스터 함수 연결
	enhancedInputComponent->BindAction(ia_booster, ETriggerEvent::Triggered, this, &APlayerFlight::StartBooster);
	enhancedInputComponent->BindAction(ia_booster, ETriggerEvent::Completed, this, &APlayerFlight::EndBooster);

	//화면 상 적 모두 없애는 함수 연결
	enhancedInputComponent->BindAction(ia_special, ETriggerEvent::Triggered, this, &APlayerFlight::SpecialFire);
	//적 강제 이동 함수 연결
	enhancedInputComponent->BindAction(ia_force, ETriggerEvent::Triggered, this, &APlayerFlight::ForceMove);

	
	////함수들을 바인딩 해줄거야. 
	////Horizontal Axis 입력에 함수 연결
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);

	////Vertical Axis 입력에 함수 연결
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	////Fire Action 입력에 함수 연결 
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	//1. 색상을 Red로 변경
	dynamicMat->SetVectorParameterValue(("MyColor"), (FVector4)FLinearColor::Red); //리니어컬러는 벡터로 캐스트 
	//2. 원래 색상을 되돌리는 함수를 바인딩한 타이머를 예약 
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}


void APlayerFlight::ChangeOriginColor()
{
	dynamicMat->SetVectorParameterValue(("MyColor"), (FVector4)initColor); //리니어컬러는 벡터로 캐스트 
	//my_mat->SetVectorParameterValue(TEXT("myColor"), initColor);
}

////좌우 입력이 있을 때 실행될 함수
//void APlayerFlight::Horizontal(float value) 
//{
//	h = value;
//	UE_LOG(LogTemp, Warning, TEXT("h = %.2f"), h);
//	direction.Y = h;
//}
//
//상하 입력이 있으르 때 실행될 함수
//void APlayerFlight::Vertical(float value)
//{
//	v = value;
//	UE_LOG(LogTemp, Warning, TEXT("v = %.2f"), v);
//	direction.Z = v;
//}


//마우스 왼쪽 버튼을 눌렀을 때 실행될 함수 
void APlayerFlight::FireBullet()
{
	//총알을 생성한다.
	//총알 블루프린트 변수 
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
	//bulletCount만큼 총알 발사
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
			//총알의 전체 간격
			float totalSize = (bulletCount - 1) * bulletSpacing;

			//기준 위치
			float base_y = totalSize * -0.5f;

			//기준위치를 오프셋 벡터를 만든다.
			FVector offset = FVector(0, base_y + bulletSpacing * i, 0);
			FVector spawnPosition = GetActorLocation() + GetActorForwardVector() * 90.0f;
			//오프셋만큼 위치를 보정
			spawnPosition += offset;

			FRotator spawnRotation = FRotator(90.0f, 0, 0);
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APlayerBullet* bullet = GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnPosition, spawnRotation, param);

			//생성된 총알(Bullet)이 bulletAngle만큼 일정하게 회전시킨다. 
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

		//총알 발사 효과음 실행 
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
	//모든 enemy를 파괴한다. 1번 방식 동적으로 바로 찾는 법
	//for (TActorIterator<AEnemy> it(GetWorld()); it; ++it) //TActorIterator로 <AEnemy>를 it(변수 선언. 아무거나 써도 됨)를 GetWorld()에서 찾고 ; 널포인트 확인하고 ; enemy를 더해줘.
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
	// 모든 enemy를 파괴한다 2번 방식(함수를 활용_
	//for (TActorIterator<AEnemy> enemy(GetWorld()); enemy; ++enemy)
	//{
	//	enemy->DestroyMySelf();
	//}
	// 2. TArray<T> 배열을 이용한 방식

	//AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());	
	//if (gm != nullptr)
	//{
	//	TArray<AEnemy*> enemyarray = gm->enemies;
	//	//for (int32 i = 0; i<gm->enemies.Num(); i++)
	//	for (int32 i = 0; i<enemyarray.Num(); i++)
	//	{
	//		//pending kill 상태 체크 
	//		if(IsValid(enemyarray[i]))
	//		{ 
	//			enemyarray[i]->DestroyMySelf();
	//		}
	//	}
	//	//리스트를 초기화 
	//	gm->enemies.Empty();
	//}

	//3. 델리게이트를 실행하는 방법
	playerBomb.Broadcast();
}

void APlayerFlight::ForceMove()
{
	forceMove.Broadcast(FVector(0, 1, 0));
}
