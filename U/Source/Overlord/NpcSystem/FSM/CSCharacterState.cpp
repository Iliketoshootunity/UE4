// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacterState.h"
#include "../Character/CSCharacter.h"
#include "../GameCharacter/CSGameCharacter.h"

UCSCharacterState::UCSCharacterState()
{

}

void UCSCharacterState::Init(int32 StateId, int32 ToSlefTransition, UFSMMachine* InMachine, UObject* InOwner)
{
	Super::Init(StateId, ToSlefTransition, InMachine, InOwner);
	Character = Cast<ACSCharacter>(InOwner);
	if (Character)
	{
		CharacterInfo = Character->GetCharacterInfo();
		CharacterType = CharacterInfo->GetCharacterType();
	}
}
