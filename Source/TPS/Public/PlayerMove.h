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

	// 마우스에 따른 회전
	void Turn(float value);
	void Lookup(float value);

	// 좌우이동
	void Horizontal(float value);
	void Vertical(float value);
	// 달리기 이벤트 처리 함수 
	void InputRun();
	void Jump();
public:
	// 걷기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float walkSpeed = 200;
	// 뛰기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float runSpeed = 600;
	// 다시 바뀔속도
	float returnSpeed = 0;
};
