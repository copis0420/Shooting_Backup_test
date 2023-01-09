// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class SHOOTING_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSetting)
	class UBoxComponent* boxComp; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSetting)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSetting)
	float moveSpeed = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = BulletSetting)
	class UParticleSystem* explosion_effect;

	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SetDirection(FVector value);

private:
	FVector direction;
	FTimerHandle lifeTimer;

	void DestroyMySelf();
};
