// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NavmeshTool.h"
#include "NavmeshToolStyle.h"
#include "NavmeshToolCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "LevelEditor.h"
#include "NavigationSystem.h"

#include "Editor.h"
#include "NavMeshExporter.h"
#include "TestNavmeshWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "EditorStyleSet.h"

static const FName NavmeshToolTabName("NavmeshTool");

#define LOCTEXT_NAMESPACE "FNavmeshToolModule"

void FNavmeshToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNavmeshToolStyle::Initialize();
	FNavmeshToolStyle::ReloadTextures();

	FNavmeshToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(FNavmeshToolCommands::Get().NavDataEnableDrawing, NavDataEnableDrawing_Custom(FGuid()));
	PluginCommands->MapAction(FNavmeshToolCommands::Get().NavDataAutoUpdate, NavDataAutoUpdate_Custom(FGuid()));

	PluginCommands->MapAction(
		FNavmeshToolCommands::Get().ExportNavigationGeom,
		FExecuteAction::CreateRaw(this, &FNavmeshToolModule::ExportNavigationGeomCliecked),
		FCanExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavmeshCanExecuteAction, FGuid()));

	PluginCommands->MapAction(
		FNavmeshToolCommands::Get().ExportNavmesh,
		FExecuteAction::CreateRaw(this, &FNavmeshToolModule::ExportNavmeshCliecked),
		FCanExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavmeshCanExecuteAction, FGuid()));

	PluginCommands->MapAction(
		FNavmeshToolCommands::Get().TestNavmesh,
		FExecuteAction::CreateRaw(this, &FNavmeshToolModule::TestNavMesh),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FNavmeshToolModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FNavmeshToolModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NavmeshToolTabName, FOnSpawnTab::CreateRaw(this, &FNavmeshToolModule::OnSpawnTestNavmeshWindow))
		.SetDisplayName(LOCTEXT("NavmeshTool", "Test Navmesh"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNavmeshToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FNavmeshToolStyle::Shutdown();

	FNavmeshToolCommands::Unregister();
}

void FNavmeshToolModule::ExportNavigationGeomCliecked()
{
	FText msg = FText::FromString(TEXT("ExportNavigationGeom Successful!"));
	NavMeshExporter::ExportNavigationGeom(msg);
	FMessageDialog::Open(EAppMsgType::Ok, msg);
}


void FNavmeshToolModule::ExportNavmeshCliecked()
{
	FText msg = FText::FromString(TEXT("ExportNavmesh Successful!"));
	NavMeshExporter::ExportNavmesh(msg);
	FMessageDialog::Open(EAppMsgType::Ok, msg);
}


void FNavmeshToolModule::TestNavMesh()
{
	FGlobalTabmanager::Get()->InvokeTab(NavmeshToolTabName);
}

bool FNavmeshToolModule::NavmeshCanExecuteAction(const FGuid SessionInstanceID) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
	return NavData != nullptr;
}

// NavDataAutoUpdate Begin
const FUIAction FNavmeshToolModule::NavDataAutoUpdate_Custom(const FGuid SessionInstanceID) const
{
	FUIAction UIAction;
	UIAction.ExecuteAction = FExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavDataAutoUpdate_Execute, SessionInstanceID);
	UIAction.CanExecuteAction = FCanExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavDataAutoUpdate_CanExecute, SessionInstanceID);
	UIAction.GetActionCheckState = FGetActionCheckState::CreateRaw(this, &FNavmeshToolModule::NavDataAutoUpdate_GetCheckState, SessionInstanceID);
	return UIAction;
}

void FNavmeshToolModule::NavDataAutoUpdate_Execute(const FGuid SessionInstanceID)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	UNavigationSystemV1::SetNavigationAutoUpdateEnabled(!NavSystem->GetIsAutoUpdateEnabled(), NavSystem);
}

bool FNavmeshToolModule::NavDataAutoUpdate_CanExecute(const FGuid SessionInstanceID) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	return NavSystem->GetIsAutoUpdateEnabled();
}

ECheckBoxState FNavmeshToolModule::NavDataAutoUpdate_GetCheckState(const FGuid SessionInstanceID) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

	return NavSystem->GetIsAutoUpdateEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
// NavDataAutoUpdate End

// NavDataEnableDrawing Begin
const FUIAction FNavmeshToolModule::NavDataEnableDrawing_Custom(const FGuid SessionInstanceID) const
{
	FUIAction UIAction;
	UIAction.ExecuteAction = FExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavDataEnableDrawing_Execute, SessionInstanceID);
	UIAction.CanExecuteAction = FCanExecuteAction::CreateRaw(this, &FNavmeshToolModule::NavDataEnableDrawing_CanExecute, SessionInstanceID);
	UIAction.GetActionCheckState = FGetActionCheckState::CreateRaw(this, &FNavmeshToolModule::NavDataEnableDrawing_GetCheckState, SessionInstanceID);
	return UIAction;
}

void FNavmeshToolModule::NavDataEnableDrawing_Execute(const FGuid SessionInstanceID)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
	if (!NavData)
		return;
	NavData->SetNavRenderingEnabled(!NavData->IsDrawingEnabled());
}

bool FNavmeshToolModule::NavDataEnableDrawing_CanExecute(const FGuid SessionInstanceID) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
	return NavData != nullptr;
}

ECheckBoxState FNavmeshToolModule::NavDataEnableDrawing_GetCheckState(const FGuid SessionInstanceID) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
	if (!NavData)
		return ECheckBoxState::Unchecked;
	return NavData->IsDrawingEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
// NavDataEnableDrawing End

void FNavmeshToolModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.BeginSection(TEXT("NavmeshTool"));
	Builder.AddSubMenu(FText::FromString("NavmeshTool"), FText::FromString("NavmeshTool"), FNewMenuDelegate::CreateLambda([](FMenuBuilder& Builder)
	{
		Builder.BeginSection("NavMeshProperties", LOCTEXT("NavMeshProperties", "Properties Setting"));
		{
			Builder.AddMenuEntry(FNavmeshToolCommands::Get().NavDataAutoUpdate);
			Builder.AddMenuEntry(FNavmeshToolCommands::Get().NavDataEnableDrawing);
		}
		Builder.EndSection();

		Builder.BeginSection(TEXT("ExportNavMesh"), LOCTEXT("ExportNavMesh", "Export Command"));
		{
			Builder.AddMenuEntry(FNavmeshToolCommands::Get().ExportNavigationGeom);
			Builder.AddMenuEntry(FNavmeshToolCommands::Get().ExportNavmesh);
		}
		Builder.EndSection();

		Builder.BeginSection(TEXT("TestNavMesh"), LOCTEXT("TestNavMesh", "Test NavMesh"));
		{
			Builder.AddMenuEntry(FNavmeshToolCommands::Get().TestNavmesh);
		}
		Builder.EndSection();
	}));
	Builder.EndSection();
}


TSharedRef<class SDockTab> FNavmeshToolModule::OnSpawnTestNavmeshWindow(const class FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(NSLOCTEXT("NavmeshTool", "NavmeshTool", "Test NavMesh"))
		[
			SNew(STestNavmeshWidget)
		];
}

void FNavmeshToolModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.BeginSection("NavmeshTool");

	Builder.AddComboButton(FUIAction(), FOnGetContent::CreateLambda([this]() -> TSharedRef<SWidget>
	{
		FMenuBuilder MenuBuilder(true, this->PluginCommands);

		MenuBuilder.BeginSection("NavMeshProperties", LOCTEXT("NavMeshProperties", "Properties Setting"));
		{
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().NavDataAutoUpdate);
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().NavDataEnableDrawing);
		}
		MenuBuilder.EndSection();

		MenuBuilder.BeginSection(TEXT("ExportNavMesh"), LOCTEXT("ExportNavMesh", "Export Command"));
		{
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().ExportNavigationGeom);
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().ExportNavmesh);
		}
		MenuBuilder.EndSection();

		MenuBuilder.BeginSection(TEXT("TestNavMesh"), LOCTEXT("TestNavMesh", "Test NavMesh"));
		{
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().TestNavmesh);
		}
		MenuBuilder.EndSection();

		return MenuBuilder.MakeWidget();

	}), LOCTEXT("NavmeshTool", "NavmeshTool"),
		LOCTEXT("NavmeshTool", "NavmeshTool"),
		FSlateIcon(FNavmeshToolStyle::GetStyleSetName(), "NavmeshTool.PluginAction"));

	Builder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNavmeshToolModule, NavmeshTool)