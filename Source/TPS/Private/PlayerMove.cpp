// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "TPS.h"

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// binding 하고싶다.

	// 초기 속도 설정
	moveComp->MaxWalkSpeed = walkSpeed;
	returnSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	moveComp->MaxWalkSpeed = FMath::Lerp(moveComp->MaxWalkSpeed, returnSpeed, 5 * DeltaTime);
}

void UPlayerMove::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &UPlayerMove::Lookup);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::Vertical);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::Jump);
}

void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::Lookup(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::Horizontal(float value)
{
	me->AddMovementInput(me->GetActorRightVector(), value);
}

void UPlayerMove::Vertical(float value)
{
	me->AddMovementInput(me->GetActorForwardVector(), value);
}

void UPlayerMove::InputRun()
{
	//auto movement = GetCharacterMovement();
	// 만약 현재 달리기 상태라면 (released)
	if (returnSpeed > walkSpeed)
	{
		// -> 걷기로 바꾸기
		returnSpeed = walkSpeed;
	}
	// 그렇지않으면 (Pressed)
	else
	{
		// -> 달리기로
		returnSpeed = runSpeed;
	}
}

void UPlayerMove::Jump()
{
	me->Jump();
}
