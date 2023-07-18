// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

// 랜덤시간 간격으로 랜덤 위치에서 적을 만들고 싶다.
// 필요속성 : 랜덤시간, 랜덤 위치, 적공장
UCLASS()
class TPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// 필요속성 : 랜덤시간, 랜덤 위치, 적공장
	UPROPERTY(EditAnywhere, Category="Spawn")
	float minTime = 1;
	UPROPERTY(EditAnywhere, Category="Spawn")
	float maxTime = 5;
	UPROPERTY(EditAnywhere, Category="Spawn")
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<class AEnemy> enemyFactory;

	// 스폰을 위한 타이머
	FTimerHandle spawnTimerHandle;

public:
	// 적 생성 함수
	void CreateEnemy();
};
