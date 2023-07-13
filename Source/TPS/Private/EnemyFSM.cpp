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

	// Ÿ��ã��
	target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));
	
	// ��
	me = Cast<AEnemy>(GetOwner());
	// 1. EnemyAnim �� �־���Ѵ�.
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	hp = initalHP;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���ݹ����� �ð�ȭ �غ���
	if (bDebugAttackRange)
	{
		DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Red, false, -1, 0, 2);
	}

	// ���� ���� ���
	FString strState;
	UEnum::GetValueAsString(mState, strState);
	//PRINT2SCREEN(TEXT("%s"), *strState);

	// FSM ����
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

// �����ð��� ������ ���¸� �̵����� �ٲٰ� �ʹ�.
// �ʿ�Ӽ� : ���ð�, ����ð�
void UEnemyFSM::IdleState()
{
	// �����ð��� ������ ���¸� �̵����� �ٲٰ� �ʹ�.
	// 1. �ð��� �귶���ϱ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ����ð��� ���ð��� �ʰ������ϱ�
	if (currentTime > idleDelayTime)
	{
		// 3. ���¸� �̵����� �ٲٰ� �ʹ�.
		mState = EEnemyState::Move;
		currentTime = 0;
		// 2. animState �� MState �� �����ϰ� �ʹ�.
		anim->animState = mState;
	}
}

// Ÿ�������� �̵��ϰ� �ʹ�.
// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UEnemyFSM::MoveState()
{
	FVector Direction = target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	me->AddMovementInput(Direction);

	// ���ݹ����ȿ� ������ ���¸� �������� ��ȯ�ϰ� �ʹ�.
	// -> transition ����
	// 3. ���� Ÿ�ٰ��� �Ÿ��� �־���Ѵ�.
	// 2. ���ݹ����ȿ� ���ͼ�
	// -> ���� Ÿ�ٰ��� �Ÿ��� ���ݹ������� �۴ٸ�
	if (distance < attackRange)
	{
		// 1. ���¸� �������� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;
		anim->animState = mState;
		currentTime = attackDelayTime;
	}

	

	// P = P0 + vt
	/*FVector P = me->GetActorLocation() + Direction * speed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);*/

	//Direction.Z = 0;
	//// Ÿ�ٹ������� ȸ���ϱ�
	//// forward vector �� d(irection �� �ǰ� �ʹ�.
	//FVector forward = me->GetActorForwardVector();
	//forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);

	//// Enemy forward ���Ͱ� direction �������� ��ġ��Ű��ʹ�.
	//me->SetActorRotation(forward.Rotation());
} 

// �����ð��� �ѹ��� �����ϰ� �ʹ�.
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));
		anim->bAttackPlay = true;
	}

	// Ÿ���� �������� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	float distance = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());
	// -> Ÿ���� ���ݹ����� �����
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}

// �����ð� ��ٷȴٰ� ���¸� ���� ��ȯ�ϰ� �ʹ�.
void UEnemyFSM::DamageState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ����ð��� ���ð��� �ʰ������ϱ�
	if (currentTime > damageDelayTime)
	{
		// 3. ���¸� ���� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}

// �Ʒ��� ��� �̵��ϴٰ� ���� �Ⱥ��̴� �������� ��������
// ���ְ� �ʹ�.
// �ʿ�Ӽ� : �Ʒ��� �̵��ϴ� �ӵ�
void UEnemyFSM::DieState()
{
	//�Ʒ��� ��� �̵��ϴٰ� P = P0 + vt
	FVector P = me->GetActorLocation() + FVector::DownVector * dieMoveSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);
	//���� �Ⱥ��̴� �������� ��������
	// Z ���� -100 �Ʒ��� ��������
	if (P.Z < -100)
	{
		// ���ְ� �ʹ�.
		me->Destroy();
	}
}

// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
// �ǰ��� �޾��� �� 
void UEnemyFSM::OnDamageProcess()
{
	// ü���� ���ҽ�Ű��
	hp--;

	currentTime = 0;
	// ü���� 0 �̻��̸�
	if (hp > 0)
	{
		// ���¸� �ǰ����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Damage;
		// �ִϸ��̼� �ǰ����� ������ȯ
		anim->bDamage = true;
		// �ǰ� �ִϸ��̼Ǹ�Ÿ�� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
		
	}
	// �׷��� ������ 
	else 
	{
		// ���¸� Die �� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Die;
		// �ݸ��� ���ֱ�
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;
}
