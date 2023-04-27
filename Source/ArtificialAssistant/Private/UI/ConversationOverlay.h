// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Widgets/SCompoundWidget.h"

class SMultiLineEditableTextBox;
class SCanvas;
class SWindow;

DECLARE_DELEGATE_OneParam(FOnClickInterpretIntention, FString)
DECLARE_DELEGATE_OneParam(FOnClickExecute, FString)

class SConversationOverlay : public SCompoundWidget
{
public:
	static constexpr int32 DefaultMaxCharacterCount = 1024;

	SLATE_BEGIN_ARGS(SConversationOverlay)
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
		_MaxCharacterCount = DefaultMaxCharacterCount;
	}
		SLATE_ARGUMENT(TWeakPtr<SWindow>, ParentWindow)
		SLATE_ARGUMENT(FText, Content)
		SLATE_ARGUMENT(int32, MaxCharacterCount)
		SLATE_EVENT(FOnClickInterpretIntention, OnClickInterpretIntention)
		SLATE_EVENT(FOnClickExecute, OnExecuteCode)
		SLATE_EVENT(FSimpleDelegate, OnCancelClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	TWeakPtr<SWindow> GetParentWindow() const;
	void DisplayPythonCode(const FString& Code);

private:
	TSharedPtr<SWidget> CreateConversationWidget();
	TSharedPtr<SWidget> CreatePythonCodeWidget();
	TSharedPtr<SWidget> CreateButtonSectionWidget();

	TSharedPtr<SWidget> CreateConfirmButtonWidget();
	TSharedPtr<SWidget> CreateCancelButtonWidget();
	TSharedPtr<SWidget> CreateExecuteButtonWidget();

	TSharedPtr<SCanvas> OverlayCanvas;
	TWeakPtr<SWindow> ParentWindow;

	FOnClickInterpretIntention OnClickInterpretIntention;
	FOnClickExecute OnExecuteClicked;
	FSimpleDelegate OnCancelClicked;

	TSharedPtr<SMultiLineEditableTextBox> ConversationTextBox;
	TSharedPtr<SMultiLineEditableTextBox> CodeTextBox;
	TSharedPtr<STextBlock> ConversationPrologueText;
	int32 MaxCharacterCount;
};

class FConversationOverlayCommands final : public TCommands<FConversationOverlayCommands>
{
public:
	FConversationOverlayCommands();
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> Command_ShowConversationOverlay;
};