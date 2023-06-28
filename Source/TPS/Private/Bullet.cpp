// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(12.5f);
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bodyMeshComp"));
	bodyMeshComp->SetupAttachment(RootComponent);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->InitialSpeed = 5000;
	ProjectileComp->MaxSpeed = 5000;
	ProjectileComp->bShouldBounce = true;
	ProjectileComp->Bounciness = 0.5f;
	ProjectileComp->SetUpdatedComponent(collisionComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMeshComp->SetStaticMesh(TempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempMat(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (TempMat.Succeeded())
	{
		bodyMeshComp->SetMaterial(0, TempMat.Object);
	}

	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 죽을 날짜 받아놓자.....
	FTimerHandle DieTimer;
	GetWorldTimerManager().SetTimer(DieTimer, this, &ABullet::Die, 2.0f, false);
}

void ABullet::Die()
{
	Destroy();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

