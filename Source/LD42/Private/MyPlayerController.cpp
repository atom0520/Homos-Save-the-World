// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Engine/World.h"
#include "LD42GameModeBase.h"
#include "GUIWidget.h"
#include "Components/Image.h"
#include "Human.h"

AMyPlayerController::AMyPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ALD42GameModeBase>(GetWorld()->GetAuthGameMode());

	TargetGenderImage = GameMode->GUIWidget->TargetGenderImage;
}

void AMyPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GameMode->GameState != EGameState::InGame) {
		AimedHuman = nullptr;
		TargetGenderImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	int ViewportSizeX;
	int ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation;
	FVector WorldDirection;
	DeprojectScreenPositionToWorld(ViewportSizeX * CrosshairViewportPosX, ViewportSizeY*CrosshairViewportPosY, WorldLocation, WorldDirection);

	FHitResult HitResult;

	FCollisionQueryParams CollisionQueryParams;
	
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WorldLocation,
		WorldLocation + WorldDirection * CrosshairLineTracingRange,
		ECC_GameTraceChannel2,
		CollisionQueryParams)) {
		
		if (HitResult.GetActor()->IsA(AHuman::StaticClass())) {
			AimedHuman = Cast<AHuman>(HitResult.GetActor());

			if (AimedHuman->Gender == 1) {
				TargetGenderImage->SetBrushFromTexture(ManIconTexture);
			}
			else {
				TargetGenderImage->SetBrushFromTexture(WomanIconTexture);
			}

			TargetGenderImage->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			AimedHuman = nullptr;
			TargetGenderImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else {
		AimedHuman = nullptr;
		TargetGenderImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction<FIntDelegate>("Skill1", EInputEvent::IE_Pressed, this, &AMyPlayerController::TriggerSkill, 1);
	InputComponent->BindAction<FIntDelegate>("Skill2", EInputEvent::IE_Pressed, this, &AMyPlayerController::TriggerSkill, 2);
}

void AMyPlayerController::TriggerSkill(int SkillID) {
	if (GameMode->GameState != EGameState::InGame) return;

	switch (SkillID) {
	case 1:
		GameMode->ActivateCondomGiveaway();
		break;
	case 2:
		GameMode->ActivateOneChildPolicy();
		break;
	}
}
