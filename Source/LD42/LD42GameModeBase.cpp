// Fill out your copyright notice in the Description page of Project Settings.

#include "LD42GameModeBase.h"
#include "Engine/Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Human.h"
#include "Kismet/GameplayStatics.h"
#include "MyStaticLibrary.h"
#include "GUIWidget.h"
#include "Components/Button.h"
#include "Engine/SkeletalMesh.h"

ALD42GameModeBase::ALD42GameModeBase() {
	UE_LOG(LogTemp, Warning, TEXT("ALD42GameModeBase::ALD42GameModeBase"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint> HumanBlueprint(TEXT("Blueprint'/Game/Blueprints/Human_BP.Human_BP'"));
	//if (HumanBlueprint.Object) {
	//	HumanBPClass = (UClass*)HumanBlueprint.Object->GeneratedClass;
	//}
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

//
//void ALD42GameModeBase::InitGame(
//	const FString & MapName,
//	const FString & Options,
//	FString & ErrorMessage) {
//
//	Super::InitGame(MapName, Options, ErrorMessage);
//
//	UE_LOG(LogTemp, Warning, TEXT("ALD42GameModeBase::InitGame"));
//
//}

void ALD42GameModeBase::PreInitializeComponents() {

	Super::PreInitializeComponents();

	//UE_LOG(LogTemp, Warning, TEXT("ALD42GameModeBase::PreInitializeComponents"));

	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		//UE_LOG(LogTemp, Warning, TEXT("Found AStaticMeshActor: %s"), *(ActorItr->GetName()));
		if (ActorItr->GetName().Compare("Planet") == 0) {
			//UE_LOG(LogTemp, Warning, TEXT("Found Planet"));
			Planet = *ActorItr;
			break;
		}
	}
}

void ALD42GameModeBase::BeginPlay() {
	Super::BeginPlay();

	GameState = EGameState::InGame;	

	GUIWidget = CreateWidget<UGUIWidget>(GetWorld(), GUIWidgetClass);
	GUIWidget->AddToViewport(UMyStaticLibrary::GUIWidgetZOrder);
	GUIWidget->RestartButton->OnClicked.AddDynamic(this, &ALD42GameModeBase::RestartGame);

	for (int Gender = 0; Gender <= 1; Gender++) {
		for (int i = 0; i < HumanSpawnNumberPerGender; i++) {
			float RandomYaw = FMath::RandRange(-180.0f, 180.0f);
			float RandomPitch = FMath::RandRange(-180.0f, 180.0f);
			FVector HumanSpawnLocation = Planet->GetActorLocation()
				+ UKismetMathLibrary::CreateVectorFromYawPitch(RandomYaw, RandomPitch, HumanSpawnHeight);
			FActorSpawnParameters SpawnParams;
			
			AHuman* SpawnedHuman = GetWorld()->SpawnActor<AHuman>(HumanBPClass, HumanSpawnLocation, FRotator(0), SpawnParams);
			if (SpawnedHuman) {
				SpawnedHuman->Initialize(Gender, 0);
				HumanPool.Add(SpawnedHuman);
			}
		}
	}

	NumberOfMen += HumanSpawnNumberPerGender;
	NumberOfWomen += HumanSpawnNumberPerGender;
	//GUIWidget->UpdatePopulationTexts(NumberOfMen, NumberOfWomen);
	UpdatePopulationTexts();
}

void ALD42GameModeBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (GameState == EGameState::Pause) return;

	if (IsCondomGiveawayActive) {
		if (CondomGiveawayBuffTimer > 0) {
			CondomGiveawayBuffTimer -= DeltaSeconds;
		}
		else {
			IsCondomGiveawayActive = false;
		}
	}

	if (IsOneChildPolicyActive) {
		if (OneChildPolicyBuffTimer>0) {
			OneChildPolicyBuffTimer -= DeltaSeconds;
		}
		else {
			IsOneChildPolicyActive = false;
			GUIWidget->OnDeactivateOneChildPolicy();
		}
	}

	if (GameState == EGameState::GameOver) return;

	if (CondomGiveawayCTTimer > 0) {
		CondomGiveawayCTTimer -= DeltaSeconds;
		GUIWidget->UpdateSkillMaskImage(1, FMath::Max<float>(0, CondomGiveawayCTTimer) / CondomGiveawayCTDuration);
	}

	if (OneChildPolicyCTTimer > 0) {
		OneChildPolicyCTTimer -= DeltaSeconds;
		GUIWidget->UpdateSkillMaskImage(2, FMath::Max<float>(0, OneChildPolicyCTTimer) / OneChildPolicyCTDuration);
	}

	if (IsOutOfSpace()) {
		GUIWidget->OnGameOver(EGameOverType::Failure);
		GameState = EGameState::GameOver;
	}
	else {
		if (NumberOfWomen == 0) {
			GUIWidget->OnGameOver(EGameOverType::GaySuccess);
			GameState = EGameState::GameOver;
		}
		else if (NumberOfMen == 0) {
			GUIWidget->OnGameOver(EGameOverType::LesbianSuccess);
			GameState = EGameState::GameOver;
		}
	}

	if (GameState == EGameState::GameOver) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeGameAndUI.SetWidgetToFocus(GUIWidget->GetCachedWidget());
		PlayerController->SetInputMode(InputModeGameAndUI);
		PlayerController->bShowMouseCursor = true;
		return;
	}	
}

bool ALD42GameModeBase::IsOutOfSpace() {

	return (NumberOfMen + NumberOfWomen) >= MaxNumberOfHumans;
}

void ALD42GameModeBase::RestartGame() {
	UGameplayStatics::OpenLevel(GetWorld(), UMyStaticLibrary::GameMapName);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void ALD42GameModeBase::SpawnHuman(int GenderToSet, bool MatureToSet, FVector Location, FRotator Rotation) {
	for (AHuman* Human : HumanPool) {
		if (!Human->Enabled) {
			Human->Initialize(GenderToSet, MatureToSet);
			Human->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
			Human->SetActorRotation(Rotation, ETeleportType::TeleportPhysics);
			Human->SetEnabled(true);

			if (GenderToSet == 1) {
				NumberOfMen++;
			}
			else {
				NumberOfWomen++;
			}

			return;
		}
	}

	FActorSpawnParameters SpawnParams;
	AHuman* SpawnedHuman = GetWorld()->SpawnActor<AHuman>(HumanBPClass, Location, Rotation, SpawnParams);
	SpawnedHuman->Initialize(GenderToSet, MatureToSet);
	HumanPool.Add(SpawnedHuman);

	if (GenderToSet == 1) {
		NumberOfMen++;
	}
	else {
		NumberOfWomen++;
	}
}

void ALD42GameModeBase::UpdatePopulationTexts() {
	GUIWidget->NumberOfMenText = FString::FromInt(NumberOfMen);
	GUIWidget->NumberOfWomenText = FString::FromInt(NumberOfWomen);
	GUIWidget->NumberOfHumansText = FString::Printf(TEXT("%d / %d"), (NumberOfMen + NumberOfWomen), MaxNumberOfHumans);
}

void ALD42GameModeBase::ActivateCondomGiveaway() {
	UE_LOG(LogTemp, Warning, TEXT("ALD42GameModeBase::ActivateCondomGiveaway"));

	if (CondomGiveawayCTTimer>0) return;

	IsCondomGiveawayActive = true;
	CondomGiveawayBuffTimer = CondomGiveawayBuffDuration;
	CondomGiveawayCTTimer = CondomGiveawayCTDuration;
}

void ALD42GameModeBase::ActivateOneChildPolicy() {
	UE_LOG(LogTemp, Warning, TEXT("ALD42GameModeBase::ActivateOneChildPolicy"));

	if (OneChildPolicyCTTimer>0) return;

	IsOneChildPolicyActive = true;
	OneChildPolicyBuffTimer = OneChildPolicyBuffDuration;
	OneChildPolicyCTTimer = OneChildPolicyCTDuration;

	GUIWidget->OnActivateOneChildPolicy();
}