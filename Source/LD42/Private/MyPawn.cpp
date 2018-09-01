// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "LD42GameModeBase.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyStaticLibrary.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ALD42GameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorLocation = GetActorLocation();
	FVector PlanetLocation = GameMode->Planet->GetActorLocation();
	FVector ActorToPlanetOffset = PlanetLocation - GetActorLocation();
	FVector ActorToPlanetDirection = ActorToPlanetOffset.GetUnsafeNormal();
	
	FVector OldActorForwardDirection = GetActorForwardVector();
	FVector NewActorForwardDirection = OldActorForwardDirection - OldActorForwardDirection.ProjectOnToNormal(ActorToPlanetDirection);
	
	FRotator NewActorRotation = FRotationMatrix::MakeFromZX(-ActorToPlanetDirection, NewActorForwardDirection).Rotator();
	
	SetActorRotation(NewActorRotation);

	FHitResult HitResult;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ActorLocation,
		PlanetLocation,
		ECC_GameTraceChannel1,
		CollisionQueryParams)){

		if (HitResult.Distance > DistanceToGround) {
			IsOnGround = false;
			CapsuleComponent->SetPhysicsLinearVelocity(ActorToPlanetDirection * UMyStaticLibrary::GravityAcceleration * DeltaTime, true);
			//CapsuleComponent->SetPhysicsLinearVelocity(ActorToPlanetDirection * GravityAcceleration * DeltaTime, true);
			//CapsuleComponent->AddForce(ActorToPlanetDirection * GravityMultiplier);
			//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), CapsuleComponent->GetPhysicsLinearVelocity().Size());
	
		}
		else {
			IsOnGround = true;

			FVector ActorVelocity = CapsuleComponent->GetPhysicsLinearVelocity();
			if (ActorVelocity.Size() > UMyStaticLibrary::EpsilonNumber) {
				CapsuleComponent->SetPhysicsLinearVelocity(-ActorVelocity.ProjectOnToNormal(ActorToPlanetDirection), true);
			}			
			//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), CapsuleComponent->GetPhysicsLinearVelocity().Size());
		}
	}


}


