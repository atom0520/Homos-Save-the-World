// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FIntDelegate, int);

class ALD42GameModeBase;
class UImage;
class AHuman;
class AMyPawn;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class LD42_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	//void ChangeAimedHeroGender();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AHuman * AimedHuman;

private:
	AMyPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime);

	virtual void SetupInputComponent() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	
	void OnPressFireButton();
	void OnPressQuitButton();
	void OnPressSkillButton(int SkillID);

	float CrosshairViewportPosX = 0.5f;
	float CrosshairViewportPosY = 0.5f;
	float CrosshairLineTracingRange = 1000.0f;

	ALD42GameModeBase * GameMode = nullptr;

	UImage* TargetGenderImage = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D * ManIconTexture = nullptr;
	UPROPERTY(EditAnywhere)
	UTexture2D * WomanIconTexture = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase * ChangeGenderSound = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase * CondomGiveawaySound = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase * OneChildPolicySound = nullptr;

	UPROPERTY(EditAnywhere)
	float MoveInputMultiplier = 200000.0f;

	UPROPERTY(EditAnywhere)
	float TurnInputMultiplier = 8.0f;

	AMyPawn * ControlledPlayer = nullptr;

	UCameraComponent * PlayerCameraComponent = nullptr;
};
