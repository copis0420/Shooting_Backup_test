// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTING_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = EnemySettings)
	int32 TraceRate = 30;

	UPROPERTY(EditDefaultsOnly, Category = EnemySettings)
	class UParticleSystem* explosion_FX;
	

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyMySelf();

	virtual void EndPlay(const EEndPlayReason::Type EEndPlayReason) override; //오버라이드는 다시 구현하겠다는 의미. 

	UFUNCTION()
	void SetNewDirection(FVector newDir);

private:
	FVector direction;
	float moveSpeed = 500.0f;
	class APlayerFlight* target;
};
