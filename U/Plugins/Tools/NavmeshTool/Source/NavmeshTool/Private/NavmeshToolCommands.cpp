// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NavmeshToolCommands.h"

#define LOCTEXT_NAMESPACE "FNavmeshToolModule"

void FNavmeshToolCommands::RegisterCommands()
{
	UI_COMMAND(NavDataEnableDrawing, "NavDataEnableDrawing", "NavDataEnableDrawing", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(NavDataAutoUpdate, "NavDataAutoUpdate", "NavDataAutoUpdate", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(ExportNavigationGeom, "ExportNavMeshGeom(obj)", "ExportNavMeshGeom(obj)", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ExportNavmesh, "ExportNavmesh(TileMesh)", "ExportNavmesh(TileMesh)", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(TestNavmesh, "TestNavmesh", "TestNavmesh", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(Copy, "Copy", "Copy", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(Paste, "Paste", "Paste", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
