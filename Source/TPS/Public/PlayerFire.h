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

	// 총알발사 기능
	void InputFire();
	void OnEnemyHit(const FHitResult& hitInfo);

	void ChangeToGrenade();
	void ChangeToSniper();
	// 총바꾸기
	void ChangeGun(bool isGrenade);

public:
	UPROPERTY()
	class UCameraComponent* tpsCamComp;
	// 유탄총
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	// 스나이퍼건
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

public:
	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// 총사운드
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
	class USoundBase* fireSound;

	// 현재 유탄발사총을 사용중인지 기억 flag
	bool bUseGrenadeGun = false;

	// 필요속성 : 총알공장
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	class UParticleSystem* bulletEffectFactory;

	// crosshair ui 파일
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// crosshair factory로부터 만들어진 인스턴스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	class UUserWidget* crosshairUI;
};
