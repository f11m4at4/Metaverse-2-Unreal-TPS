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
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	// ���콺�� ���� ȸ��
	void Turn(float value);
	void Lookup(float value);
};
