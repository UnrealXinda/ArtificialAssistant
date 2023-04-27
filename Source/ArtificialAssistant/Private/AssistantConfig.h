// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssistantConfig.generated.h"

#if WITH_EDITORONLY_DATA

UCLASS(Config=Editor, defaultconfig, meta = (DisplayName="ArtificialAssistant Settings"))
class UAssistantConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "General", meta =(DisplayName = "ChatGPT API Key"))
	FString ChatGptApiKey;
};

#endif