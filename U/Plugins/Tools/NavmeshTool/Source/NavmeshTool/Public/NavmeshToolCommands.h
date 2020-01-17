// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NavmeshToolStyle.h"

class FNavmeshToolCommands : public TCommands<FNavmeshToolCommands>
{
public:

	FNavmeshToolCommands()
		: TCommands<FNavmeshToolCommands>(TEXT("NavmeshTool"), NSLOCTEXT("Contexts", "NavmeshTool", "NavmeshTool Plugin"), NAME_None, FNavmeshToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > NavDataAutoUpdate;
	TSharedPtr< FUICommandInfo > NavDataEnableDrawing;
	TSharedPtr< FUICommandInfo > ExportNavigationGeom;
	TSharedPtr< FUICommandInfo > ExportNavmesh;
	TSharedPtr< FUICommandInfo > TestNavmesh;
	TSharedPtr< FUICommandInfo > Copy;
	TSharedPtr< FUICommandInfo > Paste;
};
