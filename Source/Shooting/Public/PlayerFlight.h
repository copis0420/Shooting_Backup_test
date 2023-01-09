// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerFlight.generated.h"


//델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpecialFire);

//1. 벡터를 인자로 넘겨 받는 델리게이트 선언 (FDirectionModifier)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionModifier, FVector, newDir);

UCLASS()
class SHOOTING_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerFlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	TSubclassOf<class APlayerBullet> bulletFactory;


	//새로운 Input Action 선언 
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_horizontal;	
	
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_vertical;	
	
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_fire;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_booster;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_special;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* ia_force;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputMappingContext* imc_myMapping;

	void ReservationHitColor(float time); 
	void ChangeOriginColor();

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class USoundBase* firesound;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	int32 bulletCount = 1;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletSpacing = 150.0f;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletAngle = 30.0f;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	bool onTrap = true;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	FVector forceEnemy = FVector(0, 300.0f, 0);
			
	FSpecialFire playerBomb;
	FDirectionModifier forceMove;  //관습적으로 이벤트에는 앞에 On을 붙임. 

	UFUNCTION(BlueprintCallable)
	void SpecialFire();

private:
	//UFUNCTION(BlueprintCallable)
	//void Horizontal(float value);

	UFUNCTION(BlueprintCallable)
	void Horizontal(const FInputActionValue& value); //함수의 오버로드. 

	UFUNCTION(BlueprintCallable)
	void Vertical(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void FireBullet(); //bool type이어서 매개변수를 삭제.

	UFUNCTION(BlueprintCallable)
	void StartBooster();

	UFUNCTION(BlueprintCallable)
	void EndBooster();


	UFUNCTION(BlueprintCallable)
	void ForceMove();

	/*UFUNCTION(BlueprintCallable)
	void Vertical(float value);

	UFUNCTION(BlueprintCallable)
	void FireBullet(); *///preference/input에서 액션이면 매개변수를 비워둬야 함.

	float h;
	float v;
	FVector direction;	
	FLinearColor initColor;
	FTimerHandle colorTimer;	
	UMaterialInstanceDynamic* dynamicMat;
	class AEnemy* AllEnemy;
	UPROPERTY(EditDefaultsOnly, Category = ExplosionSetting)
	class UParticleSystem* explosion_effect;
};
