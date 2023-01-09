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
	//����, �ε��� ����� �Ѿ��̶�� 
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy != nullptr)
	{
		//���� �ִ� ��ġ�� ���� ����Ʈ�� ����
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_effect, enemyLoc, enemyRot, true);


		//���� ����� ������ 1�� �߰��Ѵ�. 
		//AGameModeBase* gm = GetWorld()->GetGameMode();  2���� ���� �ž�. 
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
		myGM->AddScore(40);
		UE_LOG(LogTemp, Warning, TEXT("Point:%d"), myGM->GetCurrentScore());
		//�Ѿ��� �����ϰ�
		enemy->Destroy();

		//�� �ڽŵ� �����Ѵ� 
		Destroy();
	}
}