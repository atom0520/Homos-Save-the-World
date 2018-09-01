// Fill out your copyright notice in the Description page of Project Settings.

#include "Human.h"
#include "MyStaticLibrary.h"
#include "HumanAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "LD42GameModeBase.h"
#include "Components/WidgetComponent.h"

void AHuman::BeginPlay() {
	Super::BeginPlay();
}

void AHuman::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GameMode->IsCondomGiveawayActive && Gender == 1) {
		CondomBuffComponent->SetVisibility(true);
	}
	else {
		CondomBuffComponent->SetVisibility(false);
	}
}

void AHuman::GrowUp() {
	Mature = true;
	SetActorScale3D(FVector(UMyStaticLibrary::MatureHumanScale));
	//DistanceToGround = UMyStaticLibrary::MatureHumanDistanceToGround;
}

void AHuman::Initialize(int GenderToSet, bool MatureToSet) {
	Initialized = true;
	
	Gender = GenderToSet;
	Mature = MatureToSet;
	if (Mature == true) {
		SetActorScale3D(FVector(UMyStaticLibrary::MatureHumanScale));
		//DistanceToGround = UMyStaticLibrary::MatureHumanDistanceToGround;
	}
	else {
		SetActorScale3D(FVector(UMyStaticLibrary::ImmatureHumanScale));
		//DistanceToGround = UMyStaticLibrary::ImmatureHumanDistanceToGround;
	}

	if (!IsValid(SkeletalMeshComponent)) {
		TSet<UActorComponent*> ActorComponents = GetComponents();
		for (UActorComponent* ActorComponent : ActorComponents) {
			
			if (ActorComponent->GetName() == UMyStaticLibrary::HumanMeshComponentName) {
				SkeletalMeshComponent = Cast<USkeletalMeshComponent>(ActorComponent);
				//UE_LOG(LogTemp, Warning, TEXT("Found Actor Component: %s"), *(UMyStaticLibrary::HumanMeshComponentName));
			}
			else if (ActorComponent->GetName() == UMyStaticLibrary::HumanAimColliderComponentName) {
				AimColliderComponent = Cast<UCapsuleComponent>(ActorComponent);
				//UE_LOG(LogTemp, Warning, TEXT("Found Actor Component: %s"), *(UMyStaticLibrary::HumanAimColliderComponentName));
			}
			else if (ActorComponent->GetName() == UMyStaticLibrary::HumanCondomBuffComponentName) {
				CondomBuffComponent = Cast<UWidgetComponent>(ActorComponent);
				//UE_LOG(LogTemp, Warning, TEXT("Found Actor Component: %s"), *(UMyStaticLibrary::HumanCondomBuffComponentName));
			}
		}
	}

	if (Gender == 1) {
		SkeletalMeshComponent->SetMaterial(0, GameMode->ManMeshMaterial);
	}
	else {
		SkeletalMeshComponent->SetMaterial(0, GameMode->WomanMeshMaterial);
	}

	if (!IsValid(AIController)) {
		AIController = Cast<AHumanAIController>(GetController());
	}

	AIController->Initialize();
	//OnInitialize.Broadcast(Gender, Mature);
}

void AHuman::SetEnabled(bool Value) {
	Enabled = Value;
	
	if(!Value) Initialized = false;

	SetActorTickEnabled(Value);
	
	AIController->SetEnabled(Value);

	CapsuleComponent->SetSimulatePhysics(Value);

	if(Value)	AimColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else AimColliderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorHiddenInGame(!Value);
}

void AHuman::ChangeGender() {
	Gender = 1 - Gender;

	if (Gender == 1) {
		SkeletalMeshComponent->SetMaterial(0, GameMode->ManMeshMaterial);
		GameMode->NumberOfMen++;
		GameMode->NumberOfWomen--;
	}
	else {
		SkeletalMeshComponent->SetMaterial(0, GameMode->WomanMeshMaterial);
		GameMode->NumberOfMen--;
		GameMode->NumberOfWomen++;
	}

	GameMode->UpdatePopulationTexts();
}
