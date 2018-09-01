// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyStaticLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LD42_API UMyStaticLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	static const float GravityAcceleration;

	static const float EpsilonNumber;

	static const float ImmatureHumanScale;
	static const float MatureHumanScale;

	//static const float ImmatureHumanDistanceToGround;
	//static const float MatureHumanDistanceToGround;

	static const float HumanTimeToMature;

	static const FName GameMapName;

	static const int GUIWidgetZOrder;

	static const float HumanMatingDistance;

	static const float MinHumanIdleStateTime;
	static const float MaxHumanIdleStateTime;
	static const float MinHumanWanderStateTime;
	static const float MaxHumanWanderStateTime;

	static const float HumanMoveSpeed;

	//static const int MinHumanChildNumber;
	//static const int MaxHumanChildNumber;

	static const float SpawnHumanChildRadius;

	static const FString HumanMeshComponentName;

	static const FString HumanAimColliderComponentName;

	static const FString HumanCondomBuffComponentName;

	static const FString GameOverFailureSubtitleText;

	static const FString GameOverGaySuccessSubtitleText;

	static const FString GameOverLesbianSuccessSubtitleText;

	static const float SkillImageHeight;

	static const FString OneChildPolicySlogan1;

	static const FString OneChildPolicySlogan2;

	//static const float HumanSpawnHeight;
};