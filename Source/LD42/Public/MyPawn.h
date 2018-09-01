// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class ALD42GameModeBase;

UCLASS()
class LD42_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:

	AMyPawn();

	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent = nullptr;
	
	ALD42GameModeBase * GameMode = nullptr;

	bool IsOnGround = false;

protected:

	virtual void BeginPlay() override;	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float DistanceToGround = 0;

	UPROPERTY(EditAnywhere)
	float GravityAcceleration = 1000.0;	
};
