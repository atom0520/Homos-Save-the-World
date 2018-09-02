// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LD42GameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIntDynamicMulticastDelegate, int, Type);

UENUM(BlueprintType)
enum class EGameState : uint8 {
	Pause,
	InGame,
	GameOver
};

UENUM(BlueprintType)
enum class EGameOverType: uint8 {
	Failure,
	GaySuccess,
	LesbianSuccess
};

class AHuman;

class UGUIWidget;

class AStaticMeshActor;

class UParticleSystem;

/**
 * 
 */
UCLASS()
class LD42_API ALD42GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	bool IsOutOfSpace();

	UFUNCTION(BlueprintCallable)
	void RestartGame();
	
	void SpawnHuman(int GenderToSet, bool MatureToSet, FVector Location, FRotator Rotation);

	void UpdatePopulationTexts();
	
	void ActivateCondomGiveaway();

	void ActivateOneChildPolicy();

	UPROPERTY(VisibleAnywhere)
	AStaticMeshActor * Planet = nullptr;

	//UPROPERTY(VisibleAnywhere)
	EGameState GameState = EGameState::Pause;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGUIWidget * GUIWidget = nullptr;

	UPROPERTY(EditAnywhere)
	UParticleSystem * HumanMatingParticleSystem = nullptr;

	int NumberOfWomen = 0;

	int NumberOfMen = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface * ManMeshMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialInterface * WomanMeshMaterial = nullptr;

	bool IsCondomGiveawayActive = false;
	bool IsOneChildPolicyActive = false;

	float CondomGiveawayCTTimer = 0.0f;
	float OneChildPolicyCTTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	int MinHumanChildNumber = 3;

	UPROPERTY(EditAnywhere)
	int MaxHumanChildNumber = 5;


	

private:
	ALD42GameModeBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	//virtual void InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage) override;

	virtual void PreInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	int MaxNumberOfHumans = 256;

	UPROPERTY(EditAnywhere)
	int HumanSpawnNumberPerGender = 1;

	UPROPERTY(EditAnywhere)
	float HumanSpawnHeight = 512.0f;

	UPROPERTY(BlueprintAssignable)
	FIntDynamicMulticastDelegate OnGameOver;

	UPROPERTY(VisibleAnywhere)
	TArray<AHuman*> HumanPool;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGUIWidget> GUIWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHuman> HumanBPClass;

	UPROPERTY(EditAnywhere)
	float CondomGiveawayBuffDuration = 8.0f;

	float CondomGiveawayBuffTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	float CondomGiveawayCTDuration = 16.0f;	

	UPROPERTY(EditAnywhere)
	float OneChildPolicyBuffDuration = 8.0f;

	float OneChildPolicyBuffTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	float OneChildPolicyCTDuration = 16.0f;

	UPROPERTY(EditAnywhere)
	USoundBase * FailureSound = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase * SuccessSound = nullptr;

	//UPROPERTY(EditAnywhere)
	//USoundBase * RestartGameSound = nullptr;
};
