// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

// �����ð� �������� ���� ��ġ���� ���� ����� �ʹ�.
// �ʿ�Ӽ� : �����ð�, ���� ��ġ, ������
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
	// �ʿ�Ӽ� : �����ð�, ���� ��ġ, ������
	UPROPERTY(EditAnywhere, Category="Spawn")
	float minTime = 1;
	UPROPERTY(EditAnywhere, Category="Spawn")
	float maxTime = 5;
	UPROPERTY(EditAnywhere, Category="Spawn")
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<class AEnemy> enemyFactory;

	// ������ ���� Ÿ�̸�
	FTimerHandle spawnTimerHandle;

public:
	// �� ���� �Լ�
	void CreateEnemy();
};
