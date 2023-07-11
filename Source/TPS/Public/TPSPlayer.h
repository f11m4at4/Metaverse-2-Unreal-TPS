// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

// 사용자가 발사버튼을 누르면 총알을 발사하고 싶다.
// 필요속성 : 총알공장
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
	// 필요한 컴포넌트 선언
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* tpsCamComp;
	// 유탄총
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;
	// 스나이퍼건
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperGunComp;

public:
	// 사용자 입력처리 함수
	// 좌우이동
	void Horizontal(float value);
	void Vertical(float value);
	// 마우스에 따른 회전
	void Turn(float value);
	void Lookup(float value);
	
	// 총알발사 기능
	void InputFire();

	void OnEnemyHit(const FHitResult& hitInfo);

	void ChangeToGrenade();
	void ChangeToSniper();
	// 총바꾸기
	void ChangeGun(bool isGrenade);

	// 달리기 이벤트 처리 함수 
	void InputRun();

	// 필요속성 : 총알공장
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> bulletFactory;

	// 현재 유탄발사총을 사용중인지 기억 flag
	bool bUseGrenadeGun = false;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	class UParticleSystem* bulletEffectFactory;

	// crosshair ui 파일
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// crosshair factory로부터 만들어진 인스턴스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	class UUserWidget* crosshairUI;

	// 걷기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float walkSpeed = 200;
	// 뛰기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	float runSpeed = 600;
	// 다시 바뀔속도
	float returnSpeed = 0;

};
