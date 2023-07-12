// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	EEnemyState animState;

	// 공격 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	bool bAttackPlay = false;

	// 공격 종료 이벤트가 호출되면 처리할 함수
	UFUNCTION(BlueprintCallable, Category="FSM")
	void OnAttackEndEvent();
};
