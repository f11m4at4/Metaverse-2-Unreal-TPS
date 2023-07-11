// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// speed ���� �����ϰ� �ʹ�.
	// 1. �������� ���;��Ѵ�.
	auto ownerPawn = TryGetPawnOwner();
	// 2. TPSPlayer �� �ʿ��ϴ�.
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if (player)
	{
		// 3. forward vector �� �ʿ��ϴ�.
		FVector forward = player->GetActorForwardVector();
		// 4. �ӵ��� �ʿ��ϴ�.
		FVector velocity = player->GetVelocity();
		// 5. ���� �����ϰ� �ִ� TPSPlayer �� �ӵ��� forward �������� �Ҵ��ϰ�ʹ�.
		speed = FVector::DotProduct(forward, velocity);

		// direction ���� �Ҵ��ϰ� �ʹ�.
		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(right, velocity);

		// isInAir ���� �Ҵ��ϰ� �ʹ�.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}
