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

	// �����ð� �������� ���� ��ġ���� ���� ����� �ʹ�.
	// 1. �����ð� ���ϱ�
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. �˶� ���߱�
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime, false);
}


void AEnemyManager::CreateEnemy()
{
	// ������ ��ġ�� ���� �����ϰ� �ʹ�.
	// 1. ������ ��ġ�� �־���Ѵ�.
	int index = FMath::RandRange(0, spawnPoints.Num()-1);
	// 2. �����ϰ�ʹ�.
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator());

	// 1. �����ð� ���ϱ�
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. �˶� ���߱�
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime, false);
}

void AEnemyManager::FindSpawnPoints()
{
	// �˻����� ������ ��� ���͸� ��������
	TArray<AActor*> allActor;
	// ã��
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActor);

	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		AStaticMeshActor* spawn = *It;
		// ���� �̸��� spawnpoint ��?
		if (spawn->GetName().Contains(TEXT("SpawnPoint")))
		{
			// �׷��ٸ� spawnpoints �� �ϳ��� �߰��ϱ�
			spawnPoints.Add(spawn);
		}
	}
	// �ݺ��ϸ鼭
	//for (auto spawn : allActor)
	//{
	//	// ���� �̸��� spawnpoint ��?
	//	if (spawn->GetName().Contains(TEXT("SpawnPoint")))
	//	{
	//		// �׷��ٸ� spawnpoints �� �ϳ��� �߰��ϱ�
	//		spawnPoints.Add(spawn);
	//	}
	//}
}

