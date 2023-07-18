// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>
#include <Engine/StaticMeshActor.h>

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
	
	FindSpawnPoints();

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

void AEnemyManager::FindSpawnPoints()
{
	// 검색으로 월드의 모든 액터를 가져오자
	TArray<AActor*> allActor;
	// 찾기
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActor);

	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		AStaticMeshActor* spawn = *It;
		// 너의 이름이 spawnpoint 니?
		if (spawn->GetName().Contains(TEXT("SpawnPoint")))
		{
			// 그렇다면 spawnpoints 에 하나씩 추가하기
			spawnPoints.Add(spawn);
		}
	}
	// 반복하면서
	//for (auto spawn : allActor)
	//{
	//	// 너의 이름이 spawnpoint 니?
	//	if (spawn->GetName().Contains(TEXT("SpawnPoint")))
	//	{
	//		// 그렇다면 spawnpoints 에 하나씩 추가하기
	//		spawnPoints.Add(spawn);
	//	}
	//}
}

