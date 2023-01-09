// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBullet.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AEnemyBullet : public APlayerBullet
{
	GENERATED_BODY()
	
public:
	AEnemyBullet();
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
