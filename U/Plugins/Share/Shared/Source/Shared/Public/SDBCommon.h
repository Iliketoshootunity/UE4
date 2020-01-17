// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Proc
enum EDBProc
{
	Proc_GetRoleList,
	Proc_CreateRole,
	Proc_DeleteRole,
	Proc_GetRoleBaseData,
	Proc_SetRoleBaseData,
	Proc_GetViewRoleBaseData,

	Proc_CreateDBLink,

	Proc_End
};

// PN
#define PN_GETROLELIST			"GetRoleList"
#define PN_CREATEROLE			"CreateRole"
#define PN_DELETEROLE			"DeleteRole"
#define PN_GETROLEBD			"GetRoleBaseData"
#define PN_SETROLEBD			"SetRoleBaseData"
#define PN_GETVIEWROLEBD		"GetViewRoleBaseData"
#define PN_CREATE_DBLINK		"CreateDBLink"

