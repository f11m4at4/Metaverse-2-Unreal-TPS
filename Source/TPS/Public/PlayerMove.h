// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	// ���콺�� ���� ȸ��
	void Turn(float value);
	void Lookup(float value);

	// �¿��̵�
	void Horizontal(float value);
	void Vertical(float value);
	// �޸��� �̺�Ʈ ó�� �Լ� 
	void InputRun();
	void Jump();
public:
	// �ȱ� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float walkSpeed = 200;
	// �ٱ� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float runSpeed = 600;
	// �ٽ� �ٲ�ӵ�
	float returnSpeed = 0;
};
