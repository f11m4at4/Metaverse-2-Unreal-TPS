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

	// Mesh ������ �Ҵ�
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

	// ��ź�� �߰�
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-20, 30, 110));
	}

	// �Ѿ˰��� ABullet Ÿ���� �������Ʈ �����ͼ� �Ҵ�
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
	
	// crosshair ui �ν��Ͻ� �����
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
	// ��ź�� ������϶� ��ź �߻�
	if (bUseGrenadeGun)
	{
		// �Ѿ� �߻��ϰ� �ʹ�.
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// �׷��� ������ �������۰� �߻�
	else
	{
		// LineTrace �� ���� �ε��� ������ ����ȿ�� �߻���Ű�� �ʹ�.
		// 1. �������� �ʿ��ϴ�.
		FVector startPos = tpsCamComp->GetComponentLocation();
		// 2. �������� �ʿ��ϴ�.
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 5000;
		// 3. ���� ���� ���.
		FHitResult hitInfo;
		//   -> ���� �ȸ°� �ϰ� �ʹ�.
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		// 4. ���� �ε������ϱ�
		if (bHit)
		{
			// 5. �ε��� ��ü ����������
			// �ε��� �༮�� ������Ʈ�� SimulatePhysics �� Ȱ��ȭ �Ǿ� �־���Ѵ�.
			// -> �ε��� �༮�� ������Ʈ�� �ʿ��ϴ�.
			auto hitComp = hitInfo.GetComponent();
			// -> ����Ȱ��ȭ �� �ִ��� üũ�ϰ� �ʹ�.
			if (hitComp->IsSimulatingPhysics())
			{
				// �׳༮ ����������
				// P = P0 + vt, v = v0 + at, F = ma
				FVector force = hitComp->GetMass() * tpsCamComp->GetForwardVector() * 500000;
				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}
			// 6. ����ȿ�� �߻�
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
	// �ε��� �༮�� Enemy ���
	auto enemy = hitInfo.GetActor();
	// -> �ε��� �༮���� EnemyFSM ������Ʈ�� ��û����.
	auto enemyFsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("FSM")));
	// �ݹ��Լ� ȣ�����ֱ�
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

// ����ڰ� �ѹٲٱ� ��ư�� ������ ���� �ٲٰ� �ʹ�.
void ATPSPlayer::ChangeGun(bool isGrenade)
{
	// ��ź�� ����
	gunMeshComp->SetVisibility(isGrenade);
	// ���������� ����
	sniperGunComp->SetVisibility(!isGrenade);
}

