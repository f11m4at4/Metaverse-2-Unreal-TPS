// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "EnemyAnim.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));

		
	}

	// ABP 할당
	ConstructorHelpers::FClassFinder<UEnemyAnim> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}

	// 이동방향으로 회전하기
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// FSM
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



