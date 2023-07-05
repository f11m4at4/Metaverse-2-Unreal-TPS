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

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

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

	// 총알공장 ABullet 타입의 블루프린트 가져와서 할당
	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-30, 60, 110));
		sniperGunComp->SetRelativeScale3D(FVector(0.205f));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// crosshair ui 인스턴스 만들기
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	if (crosshairUI != nullptr)
	{
		crosshairUI->AddToViewport();
	}

	ChangeToSniper();
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

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::Vertical);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &ATPSPlayer::Lookup);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenade);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniper);
}

void ATPSPlayer::Horizontal(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void ATPSPlayer::Vertical(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::Lookup(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputFire()
{
	// 유탄총 사용중일때 유탄 발사
	if (bUseGrenadeGun)
	{
		// 총알 발사하고 싶다.
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// 그렇지 않으면 스나이퍼건 발사
	else
	{
		// LineTrace 를 쏴서 부딪힌 지점에 파편효과 발생시키고 싶다.
		// 1. 시작점이 필요하다.
		FVector startPos = tpsCamComp->GetComponentLocation();
		// 2. 종료점이 필요하다.
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 5000;
		// 3. 선을 만들어서 쏜다.
		FHitResult hitInfo;
		//   -> 나는 안맞게 하고 싶다.
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		// 4. 선이 부딪혔으니까
		if (bHit)
		{
			// 5. 부딪힌 물체 날려보내기
			// 부딪힌 녀석의 컴포넌트가 SimulatePhysics 가 활성화 되어 있어야한다.
			// -> 부딪힌 녀석의 컴포넌트가 필요하다.
			auto hitComp = hitInfo.GetComponent();
			// -> 물리활성화 돼 있는지 체크하고 싶다.
			if (hitComp->IsSimulatingPhysics())
			{
				// 그녀석 날려보내자
				// P = P0 + vt, v = v0 + at, F = ma
				FVector force = hitComp->GetMass() * tpsCamComp->GetForwardVector() * 500000;
				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}
			// 6. 파편효과 발생
			FTransform trans;
			trans.SetLocation(hitInfo.ImpactPoint);
			trans.SetRotation(hitInfo.ImpactNormal.ToOrientationQuat());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, trans);

			OnEnemyHit(hitInfo);
		}
		
	}
}

void ATPSPlayer::OnEnemyHit(const FHitResult& hitInfo)
{
	// 부딪힌 녀석이 Enemy 라면
	auto enemy = hitInfo.GetActor();
	// -> 부딪힌 녀석한테 EnemyFSM 컴포넌트를 요청하자.
	auto enemyFsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("FSM")));
	// 콜백함수 호출해주기
	if (enemyFsm != nullptr)
	{
		enemyFsm->OnDamageProcess();
	}
}

void ATPSPlayer::ChangeToGrenade()
{
	bUseGrenadeGun = true;
	ChangeGun(true);

}

void ATPSPlayer::ChangeToSniper()
{
	bUseGrenadeGun = false;
	ChangeGun(false);
}

// 사용자가 총바꾸기 버튼을 누르면 총을 바꾸고 싶다.
void ATPSPlayer::ChangeGun(bool isGrenade)
{
	// 유탄총 설정
	gunMeshComp->SetVisibility(isGrenade);
	// 스나이퍼총 설정
	sniperGunComp->SetVisibility(!isGrenade);
}

