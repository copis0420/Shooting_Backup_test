// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyScore|Score", meta=(BindWidget)) //세부 카테고리 나누는 법 | 로 나눔. 
	class UTextBlock* curScore;
	
	UPROPERTY(EditAnywhere, Category = "MyScore|Score", meta = (BindWidget))
	class UTextBlock* bestScore;

	UPROPERTY(EditAnywhere, Category = "MyScore|Score", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* scoreAnim;

	void PrintCurrentScore();
};
