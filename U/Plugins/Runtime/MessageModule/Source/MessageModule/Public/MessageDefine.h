// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**/
UENUM(BlueprintType)
namespace EMessageRecipientType
{
	enum  Type
	{
		None = 0,			  //None	
		Defalut = 1,          //FString
		Actor				  //Actor
	};
}

class MESSAGEMODULE_API FMessageDefine
{
public:
	const static FString MainPlayerHPChange;
	const static FString CharacterHPChange;
	const static FString CharacterMaxHPChange;
	const static FString SetCharacterName;
};

class MESSAGEMODULE_API FNetDefine
{
public:
	const static FString ResUpdateViewMessage;
	const static FString ResPlayerEnterViewMessage;
	const static FString ResMonsterEnterViewMessage;
	const static FString ResMapObjectExitViewMessage;
	const static FString ResComboNodeMessage;
	const static FString ResComboEndNodeMessage;
	const static FString ResFightResultMessage;
	const static FString ResMoveMessage;
	const static FString ResDisconectInfo;
};
