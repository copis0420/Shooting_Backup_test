// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBullet.h"
#include "MyPlayerBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyPlayerBullet : public APlayerBullet
{
	GENERATED_BODY()

public:
	AMyPlayerBullet();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
