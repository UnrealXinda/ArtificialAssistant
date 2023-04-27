// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatGptService.h"

#include "AssistantConfig.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

FChatGptService::~FChatGptService()
{
	for (const auto& [HttpRequest, LmsRequest] : HttpRequestToLmsRequestMap)
	{
		if (HttpRequest.IsValid())
		{
			HttpRequest->OnProcessRequestComplete().Unbind();
		}
	}
}

TSharedPtr<ILmsRequestHandle> FChatGptService::SendLmsRequest(const FString& Intention, TSharedPtr<ILmsRequestHandler> Handler)
{
	static const FString Url = "https://api.openai.com/v1/completions";
	const UAssistantConfig* Config = GetDefault<UAssistantConfig>();
	const FString& ApiKey = Config->ChatGptApiKey;
	const FString AuthorizationHeader = "Bearer " + ApiKey;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", AuthorizationHeader);
	Request->SetURL(Url);

	TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject);
	RequestBody->SetStringField(TEXT("prompt"), Intention);
	RequestBody->SetStringField(TEXT("model"), TEXT("text-davinci-003"));
	RequestBody->SetNumberField(TEXT("max_tokens"), 2048);

	FString RequestBodyString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBody.ToSharedRef(), JsonWriter);
	Request->SetContentAsString(RequestBodyString);

	Request->OnProcessRequestComplete().BindRaw(this, &FChatGptService::HandleHttpResponse, Handler);
	Request->ProcessRequest();

	FLmsRequestPtr RequestPtr = MakeShared<ILmsRequestHandle>();
	HttpRequestToLmsRequestMap.Add(Request, RequestPtr);
	return RequestPtr;
}

void FChatGptService::HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess,
	TSharedPtr<ILmsRequestHandler> Handler)
{
	if (!Handler.IsValid())
	{
		return;
	}

	TSharedPtr<ILmsRequestHandle>* FindResult = HttpRequestToLmsRequestMap.Find(Request);
	if (!FindResult)
	{
		return;
	}

	TSharedPtr<ILmsRequestHandle> LmsRequest = *FindResult;
	HttpRequestToLmsRequestMap.Remove(Request);

	if (bSuccess)
	{
		const FString ResponseBody = Response->GetContentAsString();

		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseBody);
		TSharedPtr<FJsonObject> JsonObject;

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			const TArray<TSharedPtr<FJsonValue>>* ChoiceValues;
			if (JsonObject->TryGetArrayField(TEXT("choices"), ChoiceValues))
			{
				if (!ChoiceValues->IsEmpty())
				{
					const TSharedPtr<FJsonObject>* ChoiceObject;
					if ((*ChoiceValues)[0]->TryGetObject(ChoiceObject))
					{
						FString PythonCode;
						if ((*ChoiceObject)->TryGetStringField(TEXT("text"), PythonCode))
						{
							PythonCode.ReplaceEscapedCharWithCharInline();

							FLmsRequestResult Result{ELmsRequestResultCode::Success, MoveTemp(PythonCode)};
							Handler->OnHandleLmsResult(LmsRequest, MoveTemp(Result));
							return;
						}
					}
				}
			}
		}
	}

	FLmsRequestResult ErrorResult{ELmsRequestResultCode::Error};
	ErrorResult.Result = Response.IsValid() ? Response->GetContentAsString() : TEXT("Unknown failure");
	Handler->OnHandleLmsResult(LmsRequest, MoveTemp(ErrorResult));
}