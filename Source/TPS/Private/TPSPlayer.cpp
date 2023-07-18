// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerAnim.h"
#include <Camera/CameraShakeBase.h>
#include "PlayerMove.h"
#include "PlayerFire.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(GetCapsuleComponent());
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("tpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	springArmComp->SetRelativeLocation(FVector(0, 70, 60));

	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	tpsCamComp->bUsePawnControlRotation = false;

	// 유탄총 추가
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-20, 30, 110));
	}

	

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("GunPosition"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-50, 4.77f, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 93, 0));
		sniperGunComp->SetRelativeScale3D(FVector(0.205f));
	}

	JumpMaxCount = 2;

	

	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("playerMove"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("playerFire"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 딱 한번 호출되는 함수
	playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);
}












