// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ConversationOverlay.h"

#include "ArtificialAssistantStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SSplitter.h"

#define LOCTEXT_NAMESPACE "SConversationOverlay"

void SConversationOverlay::Construct(const FArguments& InArgs)
{
	ParentWindow = InArgs._ParentWindow;
	MaxCharacterCount = FMath::Max(0, InArgs._MaxCharacterCount);
	OnClickInterpretIntention = InArgs._OnClickInterpretIntention;
	OnCancelClicked = InArgs._OnCancelClicked;
	OnExecuteClicked = InArgs._OnExecuteCode;

	TSharedPtr<SOverlay> Overlay;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						SAssignNew(Overlay, SOverlay)
						.Visibility(EVisibility::SelfHitTestInvisible)
						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SBorder)
							.Padding(FMargin{24.0f, 24.0f, 24.0f, 62.0f})
							.Visibility(EVisibility::SelfHitTestInvisible)
							.BorderImage(FArtificialAssistantStyle::Get().GetBrush("ArtificialAssistant.Border"))
							.BorderBackgroundColor(FArtificialAssistantStyle::Get().GetColor("ArtificialAssistant.Content.Color"))
							.ForegroundColor(FCoreStyle::Get().GetSlateColor("InvertedForeground"))
							[
								SNew(SSplitter)
								.Orientation(Orient_Vertical)
								+SSplitter::Slot()
								.Value(0.5)
								[
									CreateConversationWidget().ToSharedRef()
								]

								+SSplitter::Slot()
								.Value(0.5)
								[
									CreatePythonCodeWidget().ToSharedRef()
								]
							]
						]

						+SOverlay::Slot()
						.VAlign(VAlign_Bottom)
						.HAlign(HAlign_Fill)
						[
							CreateButtonSectionWidget().ToSharedRef()
						]
					]
				]
			]
		]
	];
}

TWeakPtr<SWindow> SConversationOverlay::GetParentWindow() const
{
	return ParentWindow;
}

void SConversationOverlay::DisplayPythonCode(const FString& Code)
{
	CodeTextBox->SetText(FText::FromString(Code));
}

TSharedPtr<SWidget> SConversationOverlay::CreateConversationWidget()
{
	return SNew(SBox)
	.Padding(FMargin{0.0f, 0.0f, 0.0f, 8.0f})
	.MinDesiredWidth(600.0f)
	.MaxDesiredWidth(1000.0f)
	.MinDesiredHeight(200.f)
	.MaxDesiredHeight(400.f)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SNew(SBox)
			.Padding(FMargin{0.0f, 0.0f, 0.0f, 8.0f})
			[
				SAssignNew(ConversationPrologueText, STextBlock)
				.Text(LOCTEXT("ConversationPrologue", "Show me the Python code in Unreal Engine 5 to "))
			]
		]

		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ConversationTextBox, SMultiLineEditableTextBox)
			.Visibility(EVisibility::SelfHitTestInvisible)
			.AutoWrapText(true)
			.TextStyle(FArtificialAssistantStyle::Get(), "ArtificialAssistant.Content")
			.OnTextChanged_Lambda([this](const FText& Text)
			{
				FString Str = Text.ToString();
				if (Str.Len() > MaxCharacterCount)
				{
					Str.LeftInline(MaxCharacterCount);
					ConversationTextBox->SetText(FText::FromString(Str));
				}
			})
		]
	];
}

TSharedPtr<SWidget> SConversationOverlay::CreatePythonCodeWidget()
{
	return SNew(SBox)
	.Padding(FMargin{0.0f, 0.0f, 0.0f, 8.0f})
	.MinDesiredWidth(600.0f)
	.MaxDesiredWidth(1000.0f)
	.MinDesiredHeight(200.f)
	.MaxDesiredHeight(400.f)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(CodeTextBox, SMultiLineEditableTextBox)
			.Visibility(EVisibility::SelfHitTestInvisible)
			.AutoWrapText(true)
			.TextStyle(FArtificialAssistantStyle::Get(), "ArtificialAssistant.Content")
		]
	];
}

TSharedPtr<SWidget> SConversationOverlay::CreateButtonSectionWidget()
{
	return SNew(SHorizontalBox)
	+SHorizontalBox::Slot()
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Bottom)
	[
		CreateCancelButtonWidget().ToSharedRef()
	]

	+SHorizontalBox::Slot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		CreateExecuteButtonWidget().ToSharedRef()
	]

	+SHorizontalBox::Slot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		CreateConfirmButtonWidget().ToSharedRef()
	];
}

TSharedPtr<SWidget> SConversationOverlay::CreateCancelButtonWidget()
{
	return SNew(SButton)
	.OnClicked_Lambda([this]()
	{
		OnCancelClicked.ExecuteIfBound();
		return FReply::Handled();
	})
	.ButtonStyle(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButtonWrapper"))
	.ContentPadding(0.0f)
	[
		SNew(SBox)
		.Padding(24.0f)
		[
			SNew(SBorder)
			.BorderImage(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButton").Normal)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SImage)
					.Image(FArtificialAssistantStyle::Get().GetBrush("ArtificialAssistant.Navigation.BackButton"))
					.ColorAndOpacity(FLinearColor::White)
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CancelButton", "Cancel"))
					.TextStyle(FArtificialAssistantStyle::Get(), "ArtificialAssistant.Content.NavigationText")
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		]
	];
}

TSharedPtr<SWidget> SConversationOverlay::CreateExecuteButtonWidget()
{
	return SNew(SButton)
	.IsEnabled_Lambda([this]()
	{
		return !CodeTextBox->GetText().IsEmpty();
	})
	.OnClicked_Lambda([this]()
	{
		FString PythonCode = CodeTextBox->GetText().ToString();
		OnExecuteClicked.ExecuteIfBound(MoveTemp(PythonCode));
		return FReply::Handled();
	})
	.ButtonStyle(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButtonWrapper"))
	.ContentPadding(0.0f)
	[
		SNew(SBox)
		.Padding(24.0f)
		[
			SNew(SBorder)
			.BorderImage(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButton").Normal)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(4.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ExecuteButton", "Execute"))
					.TextStyle(FArtificialAssistantStyle::Get(), "ArtificialAssistant.Content.NavigationText")
					.ColorAndOpacity(FLinearColor::White)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("MeshPaint.Remove"))
					.DesiredSizeOverride(FVector2D{32.0f, 32.0f})
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		]
	];
}
TSharedPtr<SWidget> SConversationOverlay::CreateConfirmButtonWidget()
{
	return SNew(SButton)
	.OnClicked_Lambda([this]()
	{
		FString FullText = ConversationTextBox->GetText().ToString() + ConversationPrologueText->GetText().ToString();
		OnClickInterpretIntention.ExecuteIfBound(MoveTemp(FullText));
		return FReply::Handled();
	})
	.IsEnabled_Lambda([this]()
	{
		return !ConversationTextBox->GetText().IsEmpty();
	})
	.ButtonStyle(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButtonWrapper"))
	.ContentPadding(0.0f)
	[
		SNew(SBox)
		.Padding(24.0f)
		[
			SNew(SBorder)
			.BorderImage(&FArtificialAssistantStyle::Get().GetWidgetStyle<FButtonStyle>("ArtificialAssistant.Content.NavigationButton").Normal)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(4.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("InterpretButton", "Interpret"))
					.TextStyle(FArtificialAssistantStyle::Get(), "ArtificialAssistant.Content.NavigationText")
					.ColorAndOpacity(FLinearColor::White)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SImage)
					.Image(FArtificialAssistantStyle::Get().GetBrush("ArtificialAssistant.Navigation.NextButton"))
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		]
	];
}

FConversationOverlayCommands::FConversationOverlayCommands() :
	TCommands(
		TEXT("ConversationOverlayCommands"),
		NSLOCTEXT("Contexts", "ConversationOverlayCommands", "ArtificialAssistant - Conversation Overlay"),
		NAME_None,
		FAppStyle::Get().GetStyleSetName())
{
}

// UI_COMMAND takes long for the compiler to optimize
PRAGMA_DISABLE_OPTIMIZATION
void FConversationOverlayCommands::RegisterCommands()
{
	UI_COMMAND(
		Command_ShowConversationOverlay,
		"Show Conversation Overlay",
		"Shows the AritificialAssistant conversation overlay",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::A, true, false, true, false)
	);
}
PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE