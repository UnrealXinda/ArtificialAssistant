// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FArtificialAssistantStyle :
    public FSlateStyleSet
{
public:
	virtual const FName& GetStyleSetName() const override;

	static const FArtificialAssistantStyle& Get();
	static void Shutdown();

	~FArtificialAssistantStyle();

private:
	FArtificialAssistantStyle();

	static FName StyleName;
	static TUniquePtr<FArtificialAssistantStyle> Instance;
};