// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPSMsg, Log, All)

// ȣ��� �Լ���ġ, �ٹ�ȣ���� ���ڿ��� ��������
#define APPINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// ȣ��� ������ ��� ��ũ��
#define PRINT_INFO() UE_LOG(TPSMsg, Warning, TEXT("%s"), *APPINFO)

// �־��� �������� �޽����� ��� ��ũ��
#define PRINT_LOG(fmt, ...) UE_LOG(TPSMsg, Warning, TEXT("%s %s"), *APPINFO, *FString::Printf(fmt, ##__VA_ARGS__))

#define PRINT2SCREEN(fmt, ...) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(fmt, ##__VA_ARGS__))