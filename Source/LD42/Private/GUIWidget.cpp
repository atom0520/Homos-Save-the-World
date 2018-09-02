// Fill out your copyright notice in the Description page of Project Settings.

#include "GUIWidget.h"
#include "LD42GameModeBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyStaticLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UGUIWidget::NativeConstruct(){
	Super::NativeConstruct();
	
	Skill1MaskImageSlot = Cast<UCanvasPanelSlot>(Skill1MaskImage->Slot);
	Skill2MaskImageSlot = Cast<UCanvasPanelSlot>(Skill2MaskImage->Slot);
	
	Reset();
}

void UGUIWidget::Reset() {
	CrosshairImage->SetVisibility(ESlateVisibility::Visible);
	TargetGenderImage->SetVisibility(ESlateVisibility::Hidden);
	GameOverFailureTitleText->SetVisibility(ESlateVisibility::Hidden);
	GameOverSuccessTitleText->SetVisibility(ESlateVisibility::Hidden);
	GameOverSubtitleText->SetVisibility(ESlateVisibility::Hidden);
	RestartButton->SetVisibility(ESlateVisibility::Hidden);
	OneChildPolicySloganText->SetVisibility(ESlateVisibility::Hidden);
	OneChildPolicySpeakerImage->SetVisibility(ESlateVisibility::Hidden);

	UpdateSkillMaskImage(1, 0);
	UpdateSkillMaskImage(2, 0);
}

void UGUIWidget::OnGameOver(EGameOverType GameOverType) {
	TargetGenderImage->SetVisibility(ESlateVisibility::Hidden);
	CrosshairImage->SetVisibility(ESlateVisibility::Hidden);

	switch (GameOverType) {
		case EGameOverType::Failure: {
			GameOverFailureTitleText->SetVisibility(ESlateVisibility::Visible);
			GameOverSubtitleText->SetText(FText::FromString(UMyStaticLibrary::GameOverFailureSubtitleText));
			break;
		}
		case EGameOverType::GaySuccess: {
			GameOverSuccessTitleText->SetVisibility(ESlateVisibility::Visible);
			GameOverSubtitleText->SetText(FText::FromString(UMyStaticLibrary::GameOverGaySuccessSubtitleText));
			break;
		}
		case EGameOverType::LesbianSuccess: {
			GameOverSuccessTitleText->SetVisibility(ESlateVisibility::Visible);
			GameOverSubtitleText->SetText(FText::FromString(UMyStaticLibrary::GameOverLesbianSuccessSubtitleText));
			break;
		}	
	}

	UpdateSkillMaskImage(1, 1);
	UpdateSkillMaskImage(2, 1);
	GameOverSubtitleText->SetVisibility(ESlateVisibility::Visible);
	RestartButton->SetVisibility(ESlateVisibility::Visible);
}

void UGUIWidget::UpdateSkillMaskImage(int SkillID, float Percent)
{
	switch (SkillID){
		case 1: {
			FVector2D NewSlotSize = Skill1MaskImageSlot->GetSize();
			NewSlotSize.Y = Percent * UMyStaticLibrary::SkillImageHeight;
			Skill1MaskImageSlot->SetSize(NewSlotSize);
			break;
		}		
		case 2: {
			FVector2D NewSlotSize = Skill2MaskImageSlot->GetSize();
			NewSlotSize.Y = Percent * UMyStaticLibrary::SkillImageHeight;
			Skill2MaskImageSlot->SetSize(NewSlotSize);
			break;
		}		
	}
}

void UGUIWidget::OnActivateOneChildPolicy() {
	OneChildPolicySpeakerImage->SetVisibility(ESlateVisibility::Visible);
	int SloganIndex = FMath::RandRange(0, 1);
	if (SloganIndex == 0) {
		OneChildPolicySloganText->SetText(FText::FromString(UMyStaticLibrary::OneChildPolicySlogan1));
	}
	else {
		OneChildPolicySloganText->SetText(FText::FromString(UMyStaticLibrary::OneChildPolicySlogan2));
	}
	OneChildPolicySloganText->SetVisibility(ESlateVisibility::Visible);
}

void UGUIWidget::OnDeactivateOneChildPolicy() {
	OneChildPolicySpeakerImage->SetVisibility(ESlateVisibility::Hidden);

	OneChildPolicySloganText->SetVisibility(ESlateVisibility::Hidden);
}


