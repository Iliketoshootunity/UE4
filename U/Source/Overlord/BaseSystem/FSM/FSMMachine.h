// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FSMState.h"
#include "FSMMachine.generated.h"


/**
 *
 */
UCLASS()
class OVERLORD_API UFSMMachine : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY() TArray<UFSMState*> FSMStates;

	int32 CurrentStateID;
	UPROPERTY() UFSMState* CurrentState;
	int32 OldStateId;
	UPROPERTY() UFSMState* OldState;
public:
	FORCEINLINE int32 GetCurrentStateID() { return CurrentStateID; }
	FORCEINLINE UFSMState* GetCurrentState() { return CurrentState; }
	FORCEINLINE int32 GetOldStateID() { return OldStateId; }
	FORCEINLINE UFSMState* GetOldState() { return OldState; }
	template <class T = UFSMState>
	T * GetState(int32 StateID)
	{
		for (size_t i = 0; i < FSMStates.Num(); i++)
		{
			if (FSMStates[i]->GetStateId() == StateID)
			{
				T* State = Cast<T>(FSMStates[i]);
				return State;
			}
		}
		return nullptr;
	}
public:
	void AddState(UFSMState* State);

	void DeleteState(UFSMState* State);

	bool ChangeState(int32 ToStateID, bool bRepeat = true);

	void ForcibleChangeState(int32 ToStateID);

	bool CanChange(int32 Transition);
};
