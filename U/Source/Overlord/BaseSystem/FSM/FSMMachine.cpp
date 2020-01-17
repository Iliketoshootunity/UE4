// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMMachine.h"
#include "FSMState.h"

void UFSMMachine::AddState(UFSMState* State)
{
	if (State == nullptr)
	{
		return;
	}
	if (FSMStates.Num() == 0)
	{
		FSMStates.Add(State);
		CurrentState = State;
		CurrentStateID = State->GetStateId();
		return;
	}
	if (FSMStates.Contains(State))return;
	FSMStates.Add(State);
}

void UFSMMachine::DeleteState(UFSMState* State)
{
	if (FSMStates.Contains(State))
	{
		FSMStates.Remove(State);
	};
}

bool UFSMMachine::ChangeState(int32 Transition, bool bRepeat)
{
	int32 StateIdTemp = CurrentState->GetOutPutState(Transition);
	if (StateIdTemp == -1)
	{
		return false;
	}
	for (size_t i = 0; i < FSMStates.Num(); i++)
	{
		if (FSMStates[i]->GetStateId() == StateIdTemp)
		{
			if (CurrentStateID == StateIdTemp)
			{
				if (!bRepeat)
				{
					return false;
				}
			}

			OldStateId = CurrentStateID;
			OldState = CurrentState;

			CurrentStateID = StateIdTemp;
			UFSMState* StateTemp = FSMStates[i];
			if (StateTemp)
			{
				CurrentState = StateTemp;
				OldState->Exit();
				CurrentState->Enter();

				return true;
			}
		}
	}
	return false;

}

void UFSMMachine::ForcibleChangeState(int32 ToStateID)
{
	CurrentStateID = ToStateID;
	for (size_t i = 0; i < FSMStates.Num(); i++)
	{
		if (FSMStates[i]->GetStateId() == CurrentStateID)
		{
			CurrentState = FSMStates[i];
		}
	}
}

bool UFSMMachine::CanChange(int32 Transition)
{
	int32 StateIdTemp = CurrentState->GetOutPutState(Transition);
	if (StateIdTemp == CurrentStateID)return true;
	if (StateIdTemp == -1)
	{
		return false;
	}
	for (size_t i = 0; i < FSMStates.Num(); i++)
	{
		if (FSMStates[i]->GetStateId() == StateIdTemp)
		{

			UFSMState* StateTemp = FSMStates[i];
			if (StateTemp)
			{
				return true;
			}
		}
	}
	return false;
}

