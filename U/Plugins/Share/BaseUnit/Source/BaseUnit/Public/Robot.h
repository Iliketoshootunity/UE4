#pragma once

#include "CoreMinimal.h"

template<class Type, int32 MaxState = 10>
class BASEUNIT_API TRobot
{
	typedef int32(Type::*BotFun) ();

	struct _state
	{
		BotFun bf;
		int64 ulTimeOut;
		int64 ulTimeStamp;
	};

public:
	enum
	{
		bot_action_imm,
		bot_action_delay,
	};

	TRobot()
	{
		m_ptype = NULL;
		m_nTimeOutState = MaxState - 1;
		//memset(m_state, 0, sizeof(_state) * MaxState);
		FMemory::Memset(m_state, 0, sizeof(_state) * MaxState);
	}

	void Init(Type* pType)
	{
		m_ptype = pType;
		m_nCurState = 0;
	}

	int32 GetState() { return m_nCurState; }

	void SetTimeOutState(int32 nTimeOutState)
	{
		m_nTimeOutState = nTimeOutState;
	}

	void operator()(int32 nSetState = INVALID_VALUE, int32 nActionType = bot_action_imm)
	{
		if (nSetState != INVALID_VALUE)
			SwitchState(nSetState);

		if (nActionType != bot_action_imm)
			return;

		int32 nState = (m_ptype->*m_state[m_nCurState].bf)();

		if (nState != INVALID_VALUE && nState != m_nCurState)
		{
			SwitchState(nState);
		}

		if (FPlatformTime::ToMilliseconds(FPlatformTime::Cycles()) - m_state[m_nCurState].ulTimeStamp > m_state[m_nCurState].ulTimeOut)
		{
			SwitchState(m_nTimeOutState);
		}
	}

	void AddState(int32 nIndex, BotFun bf, int64 ulTimeOut)
	{
		if (nIndex < 0 || nIndex >= MaxState)
			return;

		m_state[nIndex].bf = bf;
		m_state[nIndex].ulTimeOut = ulTimeOut;
	}
private:
	void SwitchState(int32 nState)
	{
		if (nState < 0 || nState >= MaxState)
			return;

		m_nCurState = nState;
		m_state[m_nCurState].ulTimeStamp = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
	}

private:
	int32			m_nCurState;
	int32			m_nTimeOutState;
	Type*			m_ptype;
	_state			m_state[MaxState];
};
