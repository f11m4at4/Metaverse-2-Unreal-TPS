// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include <Math/UnrealMathUtility.h>
#include <DrawDebugHelpers.h>
#include "TPS.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 타겟찾기
	target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));
	
	// 나
	me = Cast<AEnemy>(GetOwner());
	// 1. EnemyAnim 가 있어야한다.
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	hp = initalHP;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 공격범위를 시각화 해보자
	if (bDebugAttackRange)
	{
		DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Red, false, -1, 0, 2);
	}

	// 현재 상태 출력
	FString strState;
	UEnum::GetValueAsString(mState, strState);
	//PRINT2SCREEN(TEXT("%s"), *strState);

	// FSM 목차
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		//DieState();
		break;
	}
}

// 일정시간이 지나면 상태를 이동으로 바꾸고 싶다.
// 필요속성 : 대기시간, 경과시간
void UEnemyFSM::IdleState()
{
	// 일정시간이 지나면 상태를 이동으로 바꾸고 싶다.
	// 1. 시간이 흘렀으니까.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 대기시간을 초과했으니까
	if (currentTime > idleDelayTime)
	{
		// 3. 상태를 이동으로 바꾸고 싶다.
		mState = EEnemyState::Move;
		currentTime = 0;
		// 2. animState 에 MState 를 대입하고 싶다.
		anim->animState = mState;
	}
}

// 타겟쪽으로 이동하고 싶다.
// 필요속성 : 타겟, 이동속도
void UEnemyFSM::MoveState()
{
	FVector Direction = target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	me->AddMovementInput(Direction);

	// 공격범위안에 들어오면 상태를 공격으로 전환하고 싶다.
	// -> transition 조건
	// 3. 나와 타겟과의 거리가 있어야한다.
	// 2. 공격범위안에 들어와서
	// -> 나와 타겟과의 거리가 공격범위보다 작다면
	if (distance < attackRange)
	{
		// 1. 상태를 공격으로 전환하고 싶다.
		mState = EEnemyState::Attack;
		anim->animState = mState;
		currentTime = attackDelayTime;
	}

	

	// P = P0 + vt
	/*FVector P = me->GetActorLocation() + Direction * speed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);*/

	//Direction.Z = 0;
	//// 타겟방향으로 회전하기
	//// forward vector 가 d(irection 이 되고 싶다.
	//FVector forward = me->GetActorForwardVector();
	//forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);

	//// Enemy forward 벡터가 direction 방향으로 일치시키고싶다.
	//me->SetActorRotation(forward.Rotation());
} 

// 일정시간에 한번씩 공격하고 싶다.
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));
		anim->bAttackPlay = true;
	}

	// 타겟이 도망가면 상태를 이동으로 전환하고 싶다.
	float distance = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());
	// -> 타겟이 공격범위를 벗어나면
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}

// 일정시간 기다렸다가 상태를 대기로 전환하고 싶다.
void UEnemyFSM::DamageState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 대기시간을 초과했으니까
	if (currentTime > damageDelayTime)
	{
		// 3. 상태를 대기로 전환하고 싶다.
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}

// 아래로 계속 이동하다가 눈에 안보이는 시점까지 내려가면
// 없애고 싶다.
// 필요속성 : 아래로 이동하는 속도
void UEnemyFSM::DieState()
{
	//아래로 계속 이동하다가 P = P0 + vt
	FVector P = me->GetActorLocation() + FVector::DownVector * dieMoveSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);
	//눈에 안보이는 시점까지 내려가면
	// Z 값이 -100 아래로 내려가면
	if (P.Z < -100)
	{
		// 없애고 싶다.
		me->Destroy();
	}
}

// 피격시 호출될 콜백(이벤트) 함수
// 피격을 받았을 때 
void UEnemyFSM::OnDamageProcess()
{
	// 체력을 감소시키자
	hp--;

	currentTime = 0;
	// 체력이 0 이상이면
	if (hp > 0)
	{
		// 상태를 피격으로 전환하고 싶다.
		mState = EEnemyState::Damage;
		// 애니메이션 피격으로 상태전환
		anim->bDamage = true;
		// 피격 애니메이션몽타주 재생
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
		
	}
	// 그렇지 않으면 
	else 
	{
		// 상태를 Die 로 전환하고 싶다.
		mState = EEnemyState::Die;
		// 콜리전 꺼주기
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;
}
