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
	//�ε��� ����� �÷��̾��� 
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if(player != nullptr)
	{
		//�÷��̾ �����Ѵ�			
		player->Destroy();

		//�޴� ������ �����ϴ� �Լ��� ����
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		player->ReservationHitColor(0.2f);

		//�� �ڽŵ� �����Ѵ� 
		Destroy();
	}
}
