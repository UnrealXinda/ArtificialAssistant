// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtificialAssistant.h"

#include "IPythonScriptPlugin.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "AI/ChatGptService.h"
#include "Misc/MessageDialog.h"
#include "UI/ConversationOverlay.h"

#define LOCTEXT_NAMESPACE "FArtificialAssistantModule"

namespace
{
	class FLmsRequestHandler : public ILmsRequestHandler
	{
	public:
		DECLARE_DELEGATE_TwoParams(FOnHandleLmsResult, TSharedPtr<ILmsRequestHandle>, const FLmsRequestResult&);

		virtual void OnHandleLmsResult(TSharedPtr<ILmsRequestHandle> Request, FLmsRequestResult Result) override
		{
			OnHandleLmsResultDelegate.ExecuteIfBound(Request, Result);
		}

		FOnHandleLmsResult& OnHandleLmsResultCallback()
		{
			return OnHandleLmsResultDelegate;
		}

	private:
		FOnHandleLmsResult OnHandleLmsResultDelegate;
	};
}

void FArtificialAssistantModule::StartupModule()
{
	LanguageModelService = MakeShareable<ILanguageModelService>(new FChatGptService);
	ILanguageModelService::RegisterService(LanguageModelService.Get());

	FLmsRequestHandler* RequestHandlerImpl = new FLmsRequestHandler;
	RequestHandlerImpl->OnHandleLmsResultCallback().BindRaw(this, &FArtificialAssistantModule::HandleLmsResult);
	RequestHandler = MakeShareable<ILmsRequestHandler>(RequestHandlerImpl);

	RegisterCommands();
}

void FArtificialAssistantModule::ShutdownModule()
{
	if (RequestHandler.IsValid())
	{
		TSharedPtr<FLmsRequestHandler> RequestHandlerImpl = StaticCastSharedPtr<FLmsRequestHandler>(RequestHandler);
		RequestHandlerImpl->OnHandleLmsResultCallback().Unbind();
	}
}

void FArtificialAssistantModule::RegisterCommands()
{
	FConversationOverlayCommands::Register();

	CommandList = MakeShared<FUICommandList>();
	CommandList->MapAction(
		FConversationOverlayCommands::Get().Command_ShowConversationOverlay,
		FExecuteAction::CreateRaw(this, &FArtificialAssistantModule::ShowConversationOverlay),
		FCanExecuteAction::CreateRaw(this, &FArtificialAssistantModule::CanShowConversationOverlay));

	UToolMenu* Menu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section = Menu->AddSection("ArtificialAssistant", LOCTEXT("ArtificialAssistantHeading", "Artificial Assistant"));

	Section.AddMenuEntryWithCommandList(
		FConversationOverlayCommands::Get().Command_ShowConversationOverlay,
		CommandList,
		LOCTEXT("ArtificialAssistantShowOverlayHeading", "Show Conversation Overlay"));

	const FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	LevelEditorModule.GetGlobalLevelEditorActions()->Append(CommandList.ToSharedRef());
}

void FArtificialAssistantModule::ShowConversationOverlay()
{
	if (ConversationOverlayWidget.IsValid())
	{
		return;
	}

	TSharedPtr<SWindow> ActiveWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	if (!ActiveWindow)
	{
		return;
	}

	// Close any already opened widgets
	CloseCurrentOverlayWidget();

	ActiveWindow->AddOverlaySlot()
	[
		SAssignNew(ConversationOverlayWidget, SConversationOverlay)
		.ParentWindow(ActiveWindow)
		.OnClickInterpretIntention_Lambda([this](FString Conversation)
		{
			CurrentRequest = ILanguageModelService::GetService().SendLmsRequest(Conversation, RequestHandler);
		})
		.OnExecuteCode_Lambda([this](FString PythonCode)
		{
			const EAppReturnType::Type MessageType = FMessageDialog::Open(
					EAppMsgType::OkCancel,
					LOCTEXT("ExecuteCode_Message", "Are you sure you want to execute the Python code?"));

			if (MessageType == EAppReturnType::Cancel)
			{
				return;
			}

			IPythonScriptPlugin* Python = IPythonScriptPlugin::Get();
			if (Python && Python->IsPythonAvailable())
			{
				Python->ExecPythonCommand(*PythonCode);
			}

		})
		.OnCancelClicked_Lambda([this]()
		{
			CloseCurrentOverlayWidget();
		})
	];
}

void FArtificialAssistantModule::CloseCurrentOverlayWidget()
{
	if (ConversationOverlayWidget)
	{
		TWeakPtr<SWindow> ParentWindow = ConversationOverlayWidget->GetParentWindow();
		if (ParentWindow.IsValid())
		{
			ParentWindow.Pin()->RemoveOverlaySlot(ConversationOverlayWidget.ToSharedRef());
		}
	}

	ConversationOverlayWidget = nullptr;
}

bool FArtificialAssistantModule::CanShowConversationOverlay()
{
	return !ConversationOverlayWidget.IsValid();
}

void FArtificialAssistantModule::HandleLmsResult(TSharedPtr<ILmsRequestHandle> Request, const FLmsRequestResult& Result)
{
	if (CurrentRequest != Request)
	{
		return;
	}

	if (ConversationOverlayWidget)
	{
		if (Result.ResultCode == ELmsRequestResultCode::Success)
		{
			const FString& PythonCode = Result.Result;
			ConversationOverlayWidget->DisplayPythonCode(PythonCode);
		}
		else
		{
			const FString ErrorMsg = FString::Printf(TEXT("Error:\n%s"), *Result.Result);
			ConversationOverlayWidget->DisplayPythonCode(ErrorMsg);
		}
	}

	CurrentRequest = nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FArtificialAssistantModule, ArtificialAssistant)