// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

// ����ڰ� �߻��ư�� ������ �Ѿ��� �߻��ϰ� �ʹ�.
// �ʿ�Ӽ� : �Ѿ˰���
UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// �ʿ��� ������Ʈ ����
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* tpsCamComp;
	// ��ź��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;
	// �������۰�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperGunComp;

public:
	// ����� �Է�ó�� �Լ�
	// �¿��̵�
	void Horizontal(float value);
	void Vertical(float value);
	// ���콺�� ���� ȸ��
	void Turn(float value);
	void Lookup(float value);
	
	// �Ѿ˹߻� ���
	void InputFire();

	void ChangeToGrenade();
	void ChangeToSniper();
	// �ѹٲٱ�
	void ChangeGun(bool isGrenade);

	// �ʿ�Ӽ� : �Ѿ˰���
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> bulletFactory;
};
