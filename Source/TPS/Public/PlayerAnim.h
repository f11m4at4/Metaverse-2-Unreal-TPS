// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	// �÷��̾� �̵��ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float direction = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	bool isInAir = false;

	// Tick ó�� �������� ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ���ݾִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage;

	void PlayAttackAnimation();
};
