// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 랜덤시간 간격으로 랜덤 위치에서 적을 만들고 싶다.
	// 1. 랜덤시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. 알람 맞추기
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime, false);
}


void AEnemyManager::CreateEnemy()
{
	// 정해진 위치에 적을 스폰하고 싶다.
	// 1. 정해진 위치가 있어야한다.
	int index = FMath::RandRange(0, spawnPoints.Num()-1);
	// 2. 스폰하고싶다.
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator());

	// 1. 랜덤시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. 알람 맞추기
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime, false);
}

