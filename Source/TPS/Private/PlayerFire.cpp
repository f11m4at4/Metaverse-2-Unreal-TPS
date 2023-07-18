// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerAnim.h"
#include "Bullet.h"
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "EnemyFSM.h"
#include <UMG/Public/Blueprint/UserWidget.h>

UPlayerFire::UPlayerFire()
{
	// �ѻ���
	ConstructorHelpers::FObjectFinder<USoundBase> TempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));

	if (TempSound.Succeeded())
	{
		fireSound = TempSound.Object;
	}

	// �Ѿ˰��� ABullet Ÿ���� �������Ʈ �����ͼ� �Ҵ�
	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	// crosshair ui �ν��Ͻ� �����
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	if (crosshairUI != nullptr)
	{
		crosshairUI->AddToViewport();
	}

	ChangeToSniper();
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenade);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniper);
}

void UPlayerFire::InputFire()
{
	// �� �߻� ���� ���
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

	// camera shake
	auto pc = GetWorld()->GetFirstPlayerController();
	pc->PlayerCameraManager->StartCameraShake(cameraShake);
	// montage play
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnimation();
	}
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
		param.AddIgnoredActor(me);
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

void UPlayerFire::OnEnemyHit(const FHitResult& hitInfo)
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

void UPlayerFire::ChangeToGrenade()
{
	bUseGrenadeGun = true;
	ChangeGun(true);

}

void UPlayerFire::ChangeToSniper()
{
	bUseGrenadeGun = false;
	ChangeGun(false);
}

// ����ڰ� �ѹٲٱ� ��ư�� ������ ���� �ٲٰ� �ʹ�.
void UPlayerFire::ChangeGun(bool isGrenade)
{
	// ��ź�� ����
	gunMeshComp->SetVisibility(isGrenade);
	// ���������� ����
	sniperGunComp->SetVisibility(!isGrenade);
}
