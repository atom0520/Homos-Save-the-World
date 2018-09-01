// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GUIWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UCanvasPanelSlot;

enum class EGameOverType : uint8;

//class ALD42GameModeBase;
/**
 * 
 */
UCLASS()
class LD42_API UGUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NumberOfHumansText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NumberOfWomenText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NumberOfMenText;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* CrosshairImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* TargetGenderImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameOverFailureTitleText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameOverSuccessTitleText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameOverSubtitleText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OneChildPolicySloganText = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* OneChildPolicySpeakerImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartButton = nullptr;
	

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UImage* Skill1Image = nullptr;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UImage* Skill2Image = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Skill1MaskImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Skill2MaskImage = nullptr;

	UCanvasPanelSlot * Skill1MaskImageSlot;
	UCanvasPanelSlot * Skill2MaskImageSlot;

	/*void UpdatePopulationTexts(int NumberOfWomen, int NumberOfMen);*/
	void OnGameOver(EGameOverType GameOverType);

	void UpdateSkillMaskImage(int SkillID, float Percent);

	void OnActivateOneChildPolicy();

	void OnDeactivateOneChildPolicy();

private:
	virtual void NativeConstruct() override;

	//ALD42GameModeBase * GameMode = nullptr;
};
