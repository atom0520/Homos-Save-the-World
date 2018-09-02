// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Engine/World.h"
#include "LD42GameModeBase.h"
#include "GUIWidget.h"
#include "Components/Image.h"
#include "Human.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "MyStaticLibrary.h"
#include "Camera/CameraComponent.h"

AMyPlayerController::AMyPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ALD42GameModeBase>(GetWorld()->GetAuthGameMode());
	
	ControlledPlayer = Cast<AMyPawn>(GetPawn());

	TSet<UActorComponent*> ControlledPlayerComponents = ControlledPlayer->GetComponents();
	for (UActorComponent* ControlledPlayerComponent : ControlledPlayerComponents){
		if (ControlledPlayerComponent->GetName() == UMyStaticLibrary::PlayerCameraComponentName) {
			PlayerCameraComponent = Cast<UCameraComponent>(ControlledPlayerComponent);
		}
	}

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

	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AMyPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AMyPlayerController::LookUp);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPressFireButton);
	InputComponent->BindAction("Quit", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPressQuitButton);
	InputComponent->BindAction<FIntDelegate>("Skill1", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPressSkillButton, 1);
	InputComponent->BindAction<FIntDelegate>("Skill2", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPressSkillButton, 2);
}


void AMyPlayerController::MoveForward(float AxisValue) {
	ControlledPlayer->CapsuleComponent->AddForce(ControlledPlayer->GetActorForwardVector()*AxisValue*MoveInputMultiplier);
}

void AMyPlayerController::MoveRight(float AxisValue) {
	ControlledPlayer->CapsuleComponent->AddForce(ControlledPlayer->GetActorRightVector()*AxisValue*MoveInputMultiplier);
}

void AMyPlayerController::Turn(float AxisValue) {
	FRotator AddedRotation = FRotator::ZeroRotator;
	AddedRotation.Yaw = TurnInputMultiplier * AxisValue;
	ControlledPlayer->AddActorLocalRotation(AddedRotation);
}

void AMyPlayerController::LookUp(float AxisValue) {
	FRotator AddedRotation = FRotator::ZeroRotator;
	AddedRotation.Pitch = TurnInputMultiplier * AxisValue;
	PlayerCameraComponent->AddLocalRotation(AddedRotation);
}

void AMyPlayerController::OnPressFireButton() {
	if (IsValid(AimedHuman)) {
		AimedHuman->ChangeGender();
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ChangeGenderSound,
			AimedHuman->GetActorLocation(),
			FRotator::ZeroRotator);
	}
}

void AMyPlayerController::OnPressQuitButton() {
	ConsoleCommand("quit");
}

void AMyPlayerController::OnPressSkillButton(int SkillID) {
	if (GameMode->GameState != EGameState::InGame) return;

	switch (SkillID) {
	case 1:
		if (GameMode->CondomGiveawayCTTimer > 0) return;

		GameMode->ActivateCondomGiveaway();

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			CondomGiveawaySound,
			//GameMode->FailureSound,
			GetPawn()->GetActorLocation(),
			FRotator::ZeroRotator);

		break;
	case 2:
		if (GameMode->OneChildPolicyCTTimer > 0) return;

		GameMode->ActivateOneChildPolicy();

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			OneChildPolicySound,
			GetPawn()->GetActorLocation(),
			FRotator::ZeroRotator);

		break;
	}
}

