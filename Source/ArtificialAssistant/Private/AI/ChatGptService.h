// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ILanguageModelService.h"
#include "Interfaces/IHttpRequest.h"

class IHttpRequest;

struct FChatGptRequest
{
	TSharedPtr<IHttpRequest> HttpRequest;
};

class FChatGptService final : public ILanguageModelService
{
public:
	virtual ~FChatGptService() override;
	virtual TSharedPtr<ILmsRequestHandle> SendLmsRequest(const FString& Intention, TSharedPtr<ILmsRequestHandler> Handler) override;

private:
	void HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess, TSharedPtr<ILmsRequestHandler> Handler);

	using FLmsRequestPtr = TSharedPtr<ILmsRequestHandle>;
	TMap<FHttpRequestPtr, FLmsRequestPtr> HttpRequestToLmsRequestMap;
};