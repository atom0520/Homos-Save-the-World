// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanAIController.h"
#include "Human.h"
#include "MyStaticLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"
#include "LD42GameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

AHumanAIController::AHumanAIController() {
	PrimaryActorTick.bCanEverTick = true;

	/*if (!IsValid(ControlledHuman)) {
		
	}*/
}

void AHumanAIController::BeginPlay() {
	Super::BeginPlay();

	//if (!IsValid(Cast<UObject>(ControlledHuman))) 

	
	//Initialize();
}

void AHumanAIController::Tick(float DeltaTime) {

	if (!Initialized) return;

	if (ControlledHuman->Mature == false) {
		if (GrowthTimer > 0) {
			GrowthTimer -= DeltaTime;
		}
		else {
			ControlledHuman->GrowUp();
		}
	}
	else {	
		if (ControlledHuman->GameMode->GameState == EGameState::InGame) {
			if (IsValid(Target) == false) {
				if (SearchForTarget() == true) {
					TransitionToAIState(EHumanAIState::Chase);
				}
			}
			else {
				TransitionToAIState(EHumanAIState::Chase);
			}
		}	
	}

	switch (AIState)
	{
		case EHumanAIState::Idle: {
			if (AIStateTimer > 0) {
				AIStateTimer -= DeltaTime;
			}
			else {
				if (ControlledHuman->IsOnGround) {
					TransitionToAIState(EHumanAIState::Wander);
				}
			
			}
			break;
		}
		case EHumanAIState::Wander: {
			if (AIStateTimer > 0) {
				AIStateTimer -= DeltaTime;

				ControlledHuman->CapsuleComponent->SetPhysicsLinearVelocity(ControlledHuman->GetActorForwardVector()*UMyStaticLibrary::HumanMoveSpeed, true);
			}
			else {
				TransitionToAIState(EHumanAIState::Idle);
			}
			break;
		}
		case EHumanAIState::Chase: {
			if (ControlledHuman->GameMode->GameState != EGameState::InGame) {
				TransitionToAIState(EHumanAIState::Idle);
				return;
			}

			if (Target == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Human AI Target shouldn't be null when in chase state!"));
				TransitionToAIState(EHumanAIState::Idle);
				return;
			}
			
			if (ControlledHuman->Gender == Target->Gender) {
				Target = nullptr;
				TransitionToAIState(EHumanAIState::Idle);
				return;
			}

			//FVector TargetLocation = Target->GetActorLocation();
			//FVector SelfLocation = ControlledHuman->GetActorLocation();

			FVector SelfUpVector = ControlledHuman->GetActorUpVector();

			FVector SelfToTargetOffset = Target->GetActorLocation() - ControlledHuman->GetActorLocation();

			FVector NewSelfForwardVector = SelfToTargetOffset - UKismetMathLibrary::ProjectVectorOnToVector(SelfToTargetOffset, SelfUpVector);

			FRotator NewSelfRotation = UKismetMathLibrary::MakeRotFromXZ(NewSelfForwardVector, SelfUpVector);

			ControlledHuman->SetActorRotation(NewSelfRotation);

			if (SelfToTargetOffset.Size() > UMyStaticLibrary::HumanMatingDistance) {
				ControlledHuman->CapsuleComponent->SetPhysicsLinearVelocity(ControlledHuman->GetActorForwardVector()*UMyStaticLibrary::HumanMoveSpeed, true);
			}
			else {
				if (ControlledHuman->GameMode->IsCondomGiveawayActive) return;

				FVector ActorVelocity = ControlledHuman->CapsuleComponent->GetPhysicsLinearVelocity();
				ControlledHuman->CapsuleComponent->SetPhysicsLinearVelocity(-ActorVelocity.ProjectOnToNormal(ControlledHuman->GetActorForwardVector()), true);

				//FVector CenterBetweenSelfAndTarget = (Target->GetActorLocation() + ControlledHuman->GetActorLocation())/2;
				FVector CenterBetweenSelfAndTarget = (ControlledHuman->AimColliderComponent->GetComponentLocation() + Target->AimColliderComponent->GetComponentLocation()) / 2;
				FTransform MatingEffectSpawnTransform(
					FRotator(0), 
					CenterBetweenSelfAndTarget,
					FVector(1));

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
					ControlledHuman->GameMode->HumanMatingParticleSystem, 
					MatingEffectSpawnTransform,
					true, 
					EPSCPoolMethod::AutoRelease);

				ControlledHuman->SetEnabled(false);
				Target->SetEnabled(false);
				
				ControlledHuman->GameMode->NumberOfMen--;
				ControlledHuman->GameMode->NumberOfWomen--;

				FVector SelfToTargetDirection = SelfToTargetOffset.GetUnsafeNormal();

				FRotator SelfRotation = ControlledHuman->GetActorRotation();
				FRotator TargetRotation = Target->GetActorRotation();

				int HumanChildNumber = 1;
				if (!ControlledHuman->GameMode->IsOneChildPolicyActive) {
					HumanChildNumber = FMath::RandRange(ControlledHuman->GameMode->MinHumanChildNumber, ControlledHuman->GameMode->MaxHumanChildNumber);
				}

				int FlipFlopIndex = FMath::RandRange(0, 1);
				FVector SpawnLocation;

				FVector PlanetLocation = ControlledHuman->GameMode->Planet->GetActorLocation();
				FVector PlanetToSelfOffset = ControlledHuman->AimColliderComponent->GetComponentLocation() - PlanetLocation;
				FVector PlanetToCenterOffset = CenterBetweenSelfAndTarget - PlanetLocation;

				for (int i = 0; i < HumanChildNumber; i++) {
					FlipFlopIndex = 1 - FlipFlopIndex;
					if (FlipFlopIndex == 1) {
						SpawnLocation = PlanetLocation
							+ UKismetMathLibrary::RotateAngleAxis(PlanetToSelfOffset, FMath::RandRange(0.0f, 180.0f), PlanetToCenterOffset);

						ControlledHuman->GameMode->SpawnHuman(1, false, SpawnLocation, SelfRotation);
					}
					else {
						SpawnLocation = PlanetLocation
							+ UKismetMathLibrary::RotateAngleAxis(PlanetToSelfOffset, FMath::RandRange(-180.0f, 0.0f), PlanetToCenterOffset);
						ControlledHuman->GameMode->SpawnHuman(0, false, SpawnLocation, TargetRotation);
					}
				}

				ControlledHuman->GameMode->UpdatePopulationTexts();
			}

			break;
		}	
	}
}

void AHumanAIController::Initialize() {
	Initialized = true;

	if (!IsValid(ControlledHuman)) {
		ControlledHuman = Cast<AHuman>(GetPawn());
	}

	GrowthTimer = UMyStaticLibrary::HumanTimeToMature;
	
	TransitionToAIState(EHumanAIState::Idle);
	
	Target = nullptr;
}

bool AHumanAIController::SearchForTarget() {
	
	FVector ActorLocation = ControlledHuman->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> FilterObjectTypes = { EObjectTypeQuery::ObjectTypeQuery3 };

	TArray<AActor*> ActorsToIgnore = { ControlledHuman };

	TArray<AActor*> OutActors;

	if (UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		ActorLocation,
		TargetSearchRange,
		FilterObjectTypes,
		AHuman::StaticClass(),
		ActorsToIgnore,
		OutActors)) {

		float ClosestTargetDistance = TargetSearchRange + 100;
		for (AActor* OutActor : OutActors)
		{
			AHuman * NearbyHuman = Cast<AHuman>(OutActor);
			if (NearbyHuman->Mature && NearbyHuman->Gender!=ControlledHuman->Gender) {
				if (IsValid(Cast<AHumanAIController>(NearbyHuman->GetController())->Target)==false) {
					float TargetDistance = FVector::Distance(ActorLocation, NearbyHuman->GetActorLocation());
					if (TargetDistance < ClosestTargetDistance) {
						Target = NearbyHuman;
						ClosestTargetDistance = TargetDistance;
					}
				};
			}
		}

		if (IsValid(Target)) {
			Cast<AHumanAIController>(Target->GetController())->Target = ControlledHuman;
			//UE_LOG(LogTemp, Warning, TEXT("Connect %s and %s"), *(ControlledHuman->GetName()), *(Target->GetName()));
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

void AHumanAIController::TransitionToAIState(EHumanAIState AIState) {

	this->AIState = AIState;
	switch (AIState)
	{
		case EHumanAIState::Idle: {
			
			FVector ActorVelocity = ControlledHuman->CapsuleComponent->GetPhysicsLinearVelocity();
			ControlledHuman->CapsuleComponent->SetPhysicsLinearVelocity(-ActorVelocity.ProjectOnToNormal(ControlledHuman->GetActorForwardVector()), true);

			AIStateTimer = FMath::RandRange(UMyStaticLibrary::MinHumanIdleStateTime, UMyStaticLibrary::MaxHumanIdleStateTime);
			break;
		}
		case EHumanAIState::Wander: {
			float YawChange = FMath::RandRange(-180.0f, 180.0f);
			ControlledHuman->AddActorLocalRotation(FRotator(0, YawChange, 0));
			AIStateTimer = FMath::RandRange(UMyStaticLibrary::MinHumanWanderStateTime, UMyStaticLibrary::MaxHumanWanderStateTime);

			break;
		}
		//case EHumanAIState::Chase: {
		//	break;
		//}
	}
}

void AHumanAIController::SetEnabled(bool Value) {
	
	if(!Value) Initialized = false;

	SetActorTickEnabled(Value);
}