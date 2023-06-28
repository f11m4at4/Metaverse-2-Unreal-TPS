// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"
// 총알이 바라보는 방향으로 이동하고싶다.
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
