#pragma once

#include "CoreMinimal.h"

class NETWORK_API IMsgProtocol
{
public:
	virtual bool ProcessMsg(int32 usProtocol, void* pMsgData, int32 length) = 0;
};