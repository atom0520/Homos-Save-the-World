// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPawn.h"
#include "Human.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIntBoolParamsDelegate, int, Gender, bool, Mature);

class AHumanAIController;
class USkeletalMeshComponent;
class UWidgetComponent;

UCLASS()
class LD42_API AHuman : public AMyPawn
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialize(int GenderToSet, bool MatureToSet);

	void GrowUp();

	void SetEnabled(bool Value);

	UFUNCTION(BlueprintCallable)
	void ChangeGender();

	UPROPERTY(EditAnywhere)
	bool Mature;
	
	UPROPERTY(EditAnywhere)
	int Gender;	

	bool Enabled = true;

	UCapsuleComponent* AimColliderComponent = nullptr;

protected:

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FIntBoolParamsDelegate OnInitialize;

	bool Initialized;

	AHumanAIController* AIController = nullptr;

	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UWidgetComponent* CondomBuffComponent = nullptr;
};
