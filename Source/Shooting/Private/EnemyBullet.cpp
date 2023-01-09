// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "Components/BoxComponent.h"
#include "PlayerFlight.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

AEnemyBullet::AEnemyBullet()
{
	boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));
}

void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}


void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//부딪힌 대상이 플레이어라면 
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if(player != nullptr)
	{
		//플레이어를 제거한다			
		player->Destroy();

		//메뉴 위젯을 생성하는 함수를 실행
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		player->ReservationHitColor(0.2f);

		//나 자신도 제거한다 
		Destroy();
	}
}
