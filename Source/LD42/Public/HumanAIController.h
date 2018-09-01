// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HumanAIController.generated.h"

class AHuman;

UENUM(BlueprintType)
enum class EHumanAIState : uint8 {
	Idle,
	Wander,
	Chase
};

/**
 * 
 */
UCLASS()
class LD42_API AHumanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHumanAIController();

	void Initialize();

	void SetEnabled(bool Value);

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	EHumanAIState AIState;
	
	UPROPERTY(VisibleAnywhere)
	AHuman * ControlledHuman = nullptr;

	UPROPERTY(VisibleAnywhere)
	AHuman * Target = nullptr;

	UPROPERTY(EditAnywhere)
	float TargetSearchRange;

	float GrowthTimer;

	//bool FoundTarget;

	UPROPERTY(VisibleAnywhere)
	float AIStateTimer;

	bool SearchForTarget();

	void TransitionToAIState(EHumanAIState AIState);

	bool Initialized;


};
