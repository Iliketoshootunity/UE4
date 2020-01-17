// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FNavmeshToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FNavmeshToolModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FNavmeshToolModule>("NavmeshTool");
	}
public:
	inline TSharedPtr<class FUICommandList> GetUICommandList()
	{
		return PluginCommands;
	}
	/** ExportNavMesh Button */
public:
	void ExportNavigationGeomCliecked();
	void ExportNavmeshCliecked();
	void TestNavMesh();

protected:
	bool NavmeshCanExecuteAction(const FGuid SessionInstanceID) const;
	/** NavDataAutoUpdate ToggleButton */
public:
	const FUIAction NavDataAutoUpdate_Custom(const FGuid SessionInstanceID) const;
protected:
	void NavDataAutoUpdate_Execute(const FGuid SessionInstanceID);
	bool NavDataAutoUpdate_CanExecute(const FGuid SessionInstanceID) const;
	ECheckBoxState NavDataAutoUpdate_GetCheckState(const FGuid SessionInstanceID) const;


	/** NavDataEnableDrawing ToggleButton */
public:
	const FUIAction NavDataEnableDrawing_Custom(const FGuid SessionInstanceID) const;
protected:
	void NavDataEnableDrawing_Execute(const FGuid SessionInstanceID);
	bool NavDataEnableDrawing_CanExecute(const FGuid SessionInstanceID) const;
	ECheckBoxState NavDataEnableDrawing_GetCheckState(const FGuid SessionInstanceID) const;
	
private:
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnTestNavmeshWindow(const class FSpawnTabArgs& SpawnTabArgs);
private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
