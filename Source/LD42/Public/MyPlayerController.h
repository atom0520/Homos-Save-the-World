// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FIntDelegate, int);

class ALD42GameModeBase;
class UImage;
class AHuman;
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

	float CrosshairViewportPosX = 0.5f;
	float CrosshairViewportPosY = 0.5f;
	float CrosshairLineTracingRange = 1000.0f;

	ALD42GameModeBase * GameMode = nullptr;

	UImage* TargetGenderImage;

	UPROPERTY(EditAnywhere)
	UTexture2D * ManIconTexture;
	UPROPERTY(EditAnywhere)
	UTexture2D * WomanIconTexture;

	void TriggerSkill(int SkillID);
};
