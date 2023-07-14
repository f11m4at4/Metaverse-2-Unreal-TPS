// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "TPS.h"

void UEnemyAnim::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEndNoti()
{
	// 이제 DieState 함수 재생해라.
	isDiePlaying = true;
}

