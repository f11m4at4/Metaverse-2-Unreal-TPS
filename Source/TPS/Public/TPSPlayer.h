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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerFire;

public:
	// 사용자 입력처리 함수

};
