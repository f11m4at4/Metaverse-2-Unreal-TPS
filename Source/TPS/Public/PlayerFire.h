// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	// �Ѿ˹߻� ���
	void InputFire();
	void OnEnemyHit(const FHitResult& hitInfo);

	void ChangeToGrenade();
	void ChangeToSniper();
	// �ѹٲٱ�
	void ChangeGun(bool isGrenade);

public:
	UPROPERTY()
	class UCameraComponent* tpsCamComp;
	// ��ź��
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	// �������۰�
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

public:
	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// �ѻ���
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
	class USoundBase* fireSound;

	// ���� ��ź�߻����� ��������� ��� flag
	bool bUseGrenadeGun = false;

	// �ʿ�Ӽ� : �Ѿ˰���
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	class UParticleSystem* bulletEffectFactory;

	// crosshair ui ����
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// crosshair factory�κ��� ������� �ν��Ͻ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	class UUserWidget* crosshairUI;
};
