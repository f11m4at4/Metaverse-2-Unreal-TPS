// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"
// �Ѿ��� �ٶ󺸴� �������� �̵��ϰ�ʹ�.
UCLASS()
class TPS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMeshComp;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileComp;
};
