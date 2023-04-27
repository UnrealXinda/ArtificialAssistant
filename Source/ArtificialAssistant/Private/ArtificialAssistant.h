// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IArtificialAssistant.h"
#include "AI/ILanguageModelService.h"

class SConversationOverlay;

class FArtificialAssistantModule final : public IArtificialAssistantModule
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterCommands();
	void ShowConversationOverlay();
	void CloseCurrentOverlayWidget();
	bool CanShowConversationOverlay();

	void HandleLmsResult(TSharedPtr<ILmsRequestHandle> Request, const FLmsRequestResult& Result);

	TSharedPtr<ILanguageModelService> LanguageModelService;
	TSharedPtr<FUICommandList> CommandList;
	TSharedPtr<SConversationOverlay> ConversationOverlayWidget;

	TSharedPtr<ILmsRequestHandle> CurrentRequest;
	TSharedPtr<ILmsRequestHandler> RequestHandler;
};