// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// enemy �� ���¸� ��Ÿ�� enum �ڷ��� �����
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Move,
	Attack,
	Damage,
	Die
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	EEnemyState mState = EEnemyState::Idle;

	// �ʿ�Ӽ� : ���ð�, ����ð�
	UPROPERTY(EditAnywhere, Category="FSM")
	float idleDelayTime = 2;
	float currentTime = 0;

	// ------------------------------------------- //
	// �̵�
	// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
	UPROPERTY()
	class ATPSPlayer* target;
	UPROPERTY(EditAnywhere, Category="FSM")
	float speed = 500;
	// ���� �����ϰ� �ִ� ����
	UPROPERTY()
	class AEnemy* me;


	// �ʿ�Ӽ� : ���ݹ���
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackRange = 2;

	// ���ݹ��� �ð�ȭ����
	UPROPERTY(EditAnywhere, Category="FSM")
	bool bDebugAttackRange = false;

	// ------------------------------------------- //
	// ����
	// �ʿ�Ӽ� : ���ݴ��ð�
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackDelayTime = 2;

public:	// ���� �Լ�
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
	void OnDamageProcess();
};
