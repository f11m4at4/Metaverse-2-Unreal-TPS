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
	// 총사운드
	ConstructorHelpers::FObjectFinder<USoundBase> TempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));

	if (TempSound.Succeeded())
	{
		fireSound = TempSound.Object;
	}

	// 총알공장 ABullet 타입의 블루프린트 가져와서 할당
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

	// crosshair ui 인스턴스 만들기
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
	// 총 발사 사운드 재생
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
		param.AddIgnoredActor(me);
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

void UPlayerFire::OnEnemyHit(const FHitResult& hitInfo)
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

// 사용자가 총바꾸기 버튼을 누르면 총을 바꾸고 싶다.
void UPlayerFire::ChangeGun(bool isGrenade)
{
	// 유탄총 설정
	gunMeshComp->SetVisibility(isGrenade);
	// 스나이퍼총 설정
	sniperGunComp->SetVisibility(!isGrenade);
}
