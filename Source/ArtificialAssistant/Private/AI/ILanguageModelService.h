// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Language Model Service Result Code
enum class ELmsRequestResultCode
{
	Success,
	Error
};

struct ILmsRequestHandle
{
};

struct FLmsRequestResult
{
	ELmsRequestResultCode ResultCode;
	FString Result;
};

class ILmsRequestHandler
{
public:
	virtual ~ILmsRequestHandler() = default;
	virtual void OnHandleLmsResult(TSharedPtr<ILmsRequestHandle> Request, FLmsRequestResult Result) = 0;
};

class ILanguageModelService
{
public:
	static ILanguageModelService& GetService()
	{
		return *GService;
	}

	static void RegisterService(ILanguageModelService* Service)
	{
		check(Service)
		GService = Service;
	}

	virtual ~ILanguageModelService() = default;
	virtual TSharedPtr<ILmsRequestHandle> SendLmsRequest(const FString& Intention, TSharedPtr<ILmsRequestHandler> Handler) = 0;

private:
	static inline ILanguageModelService* GService = nullptr;
};