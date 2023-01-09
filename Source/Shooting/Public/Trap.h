// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SHOOTING_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//USpehereComponent�� ��Ʈ �ݸ��� ����
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphComp;

	//UStaticMesh�� ��Ʈ�� �ڽ� ������Ʈ�� ���� 
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	//"InTrap �Լ�"
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);UFUNCTION()
	void OffOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	float currentTime = 0.0f;
	float teleportTime = 3.0f;	
};
