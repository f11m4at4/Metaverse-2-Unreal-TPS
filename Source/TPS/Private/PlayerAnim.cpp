// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// speed 값을 세팅하고 싶다.
	// 1. 소유폰을 얻어와야한다.
	auto ownerPawn = TryGetPawnOwner();
	// 2. TPSPlayer 가 필요하다.
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if (player)
	{
		// 3. forward vector 가 필요하다.
		FVector forward = player->GetActorForwardVector();
		// 4. 속도가 필요하다.
		FVector velocity = player->GetVelocity();
		// 5. 나를 소유하고 있는 TPSPlayer 의 속도와 forward 내적값을 할당하고싶다.
		speed = FVector::DotProduct(forward, velocity);

		// direction 값을 할당하고 싶다.
		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(right, velocity);

		// isInAir 값을 할당하고 싶다.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}
