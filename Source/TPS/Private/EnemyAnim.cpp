// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "TPS.h"

void UEnemyAnim::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEndNoti()
{
	// ���� DieState �Լ� ����ض�.
	isDiePlaying = true;
}

