// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerBullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

AMyPlayerBullet::AMyPlayerBullet()
{
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
}

void AMyPlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	//SetDirection(GetActorForwardVector().GetSafeNormal());
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayerBullet::OnOverlap);
}


void AMyPlayerBullet::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만일, 부딪힌 대상이 총알이라면 
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy != nullptr)
	{
		//적이 있던 위치에 폭발 이펙트를 생성
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);


		//게임 모드의 점수를 1점 추가한다. 
		//AGameModeBase* gm = GetWorld()->GetGameMode();  2개는 같은 거야. 
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
		myGM->AddScore(40);
		UE_LOG(LogTemp, Warning, TEXT("Point:%d"), myGM->GetCurrentScore());
		//총알을 제거하고
		enemy->Destroy();

		//나 자신도 제거한다 
		Destroy();
	}
}