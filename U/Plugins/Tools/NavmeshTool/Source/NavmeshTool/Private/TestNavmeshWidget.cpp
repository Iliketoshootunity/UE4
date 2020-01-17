// Fill out your copyright notice in the Description page of Project Settings.

#include "TestNavmeshWidget.h"
#include "SlateOptMacros.h"
#include "Editor.h"
#include "NavigationSystem.h"
#include "NavmeshTool.h"
#include "NavmeshToolCommands.h"
#include "UICommandList.h"
#include "WindowsPlatformApplicationMisc.h"
#include "SEntryBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "DetailLayoutBuilder.h"
#include "SBox.h"
#include "SCheckBox.h"
#include "SButton.h"
#include "SScrollBox.h"
#include "STextBlock.h"
#include "SColorBlock.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Engine/TextRenderActor.h"
#include "AppFramework/Public/Widgets/Colors/SColorPicker.h"
#include "WidgetPath.h"
#include "SlateApplication.h"
#include "RecastHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EditorViewportClient.h"
#include "Editor/EditorEngine.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "FileManager.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
#define LOCTEXT_NAMESPACE "STestNavmeshWidget"

TArray<FNavPathPoint>		STestNavmeshWidget::m_TPathArray;
TArray<ATextRenderActor*>	STestNavmeshWidget::m_TRenderTextArray;
FLinearColor STestNavmeshWidget::m_PathPickerColor = FLinearColor::White;
FLinearColor STestNavmeshWidget::m_LogPickerColor = FLinearColor::White;
FString STestNavmeshWidget::m_ShowUnrealWidgetName = TEXT("UnrealCoordinate");
bool STestNavmeshWidget::m_bRefreshPrintfWayPoint = false;
FVector STestNavmeshWidget::m_BeginLocatin = FVector::ZeroVector;
FVector STestNavmeshWidget::m_EndLocation = FVector::ZeroVector;

void STestNavmeshWidget::Construct(const FArguments& InArgs)
{
	if (!m_HoveredImageStyle.IsValid())
	{
		m_HoveredImageStyle = MakeShareable(new FSlateBrush);
		m_HoveredImageStyle->DrawAs = ESlateBrushDrawType::Image;
		m_HoveredImageStyle->Tiling = ESlateBrushTileType::NoTile;
		m_HoveredImageStyle->ImageType = ESlateBrushImageType::FullColor;
		m_HoveredImageStyle->TintColor = FLinearColor(1.0f, 0.5f, 0.0f, 1.f);
	}

	if (!m_UnHoveredImageStyle.IsValid())
	{
		m_UnHoveredImageStyle = MakeShareable(new FSlateBrush);
		m_UnHoveredImageStyle->DrawAs = ESlateBrushDrawType::Image;
		m_UnHoveredImageStyle->Tiling = ESlateBrushTileType::NoTile;
		m_UnHoveredImageStyle->ImageType = ESlateBrushImageType::FullColor;
		m_UnHoveredImageStyle->TintColor = FLinearColor::Black;
	}
	TSharedPtr<FUICommandList> UICommandList = FNavmeshToolModule::Get().GetUICommandList();
	if (UICommandList.IsValid())
	{
		UICommandList->MapAction(
			FNavmeshToolCommands::Get().Copy,
			FExecuteAction::CreateRaw(this, &STestNavmeshWidget::OnCopyLocation),
			FCanExecuteAction());

		UICommandList->MapAction(
			FNavmeshToolCommands::Get().Paste,
			FExecuteAction::CreateRaw(this, &STestNavmeshWidget::OnPasteLocation),
			FCanExecuteAction());
	}

	FSlateFontInfo FontInfo = IDetailLayoutBuilder::GetDetailFont();

	TSharedPtr<SCheckBox> ShowLevelCheckBox;
	TSharedPtr<SCheckBox> UnrealCoordinateCheckBox;

	ChildSlot
		[
			SNew(SVerticalBox)
			.Visibility(EVisibility::SelfHitTestInvisible)

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(0.0f, 2.0f)
			.AutoHeight()
			[
				BuildChildWidget(m_BeginEntryBlockWidget, TEXT("Begin Point:"))
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(0.0f, 2.0f)
			.AutoHeight()
			[
				BuildChildWidget(m_EndEntryBlockWidget, TEXT("End    Point:"))
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(0.0f, 2.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				.Visibility(EVisibility::SelfHitTestInvisible)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SButton)
					.TextFlowDirection(ETextFlowDirection::LeftToRight)
					.TextShapingMethod(ETextShapingMethod::FullShaping)
					.Text(FText::FromString(TEXT("Export TestNavmesh")))
					.OnClicked(this, &STestNavmeshWidget::OnExportTestNavmesh)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					// Just take up space for alignment
					SNew(SBox)
					.WidthOverride(18.0f)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SButton)
					.TextFlowDirection(ETextFlowDirection::LeftToRight)
					.TextShapingMethod(ETextShapingMethod::FullShaping)
					.Text(FText::FromString(TEXT("Find Paths")))
					.OnClicked(this, &STestNavmeshWidget::OnFindPath)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					// Just take up space for alignment
					SNew(SBox)
					.WidthOverride(18.0f)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SButton)
					.TextFlowDirection(ETextFlowDirection::LeftToRight)
					.TextShapingMethod(ETextShapingMethod::FullShaping)
					.Text(FText::FromString(TEXT("Clear Paths")))
					.OnClicked(this, &STestNavmeshWidget::OnClearPath)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					// Just take up space for alignment
					SNew(SBox)
					.WidthOverride(18.0f)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SHorizontalBox)
					.Visibility(EVisibility::SelfHitTestInvisible)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Path Color:")))
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						// Just take up space for alignment
						SNew(SBox)
						.WidthOverride(4.0f)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SAssignNew(m_PathColorPickerWidget, SColorBlock)
						.Color(this, &STestNavmeshWidget::OnGetPathColorForColorBlock)
						.ShowBackgroundForAlpha(true)
						.IgnoreAlpha(false)
						.OnMouseButtonDown(this, &STestNavmeshWidget::OnMouseButtonDownPathColorBlock)
						.Size(FVector2D(18.0f, 18.0f))
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					// Just take up space for alignment
					SNew(SBox)
					.WidthOverride(18.0f)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(ShowLevelCheckBox, SCheckBox)
					.HAlign(HAlign_Fill)
					.IsChecked(ECheckBoxState::Checked)
					.Type(ESlateCheckBoxType::CheckBox)
					.CheckBoxContentUsesAutoWidth(true)
					.OnCheckStateChanged(this, &STestNavmeshWidget::OnShowToLevelCheckStateChanged)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("ShowPathsToLevel")))
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					// Just take up space for alignment
					SNew(SBox)
					.WidthOverride(18.0f)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(UnrealCoordinateCheckBox, SCheckBox)
					.HAlign(HAlign_Fill)
					.IsChecked(ECheckBoxState::Checked)
					.Type(ESlateCheckBoxType::CheckBox)
					.CheckBoxContentUsesAutoWidth(true)
					.OnCheckStateChanged(this, &STestNavmeshWidget::OnUeCoordinateCheckStateChanged)
					[
						SAssignNew(m_UCCheckBoxTextBlock, STextBlock)
						.Text(FText::FromString(m_ShowUnrealWidgetName))
					]
				]

			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.Padding(0.0f, 18.0f)
			.FillHeight(1.0f)
			[
				SNew(SVerticalBox)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.Padding(0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					.Visibility(EVisibility::SelfHitTestInvisible)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Logs:")))
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						// Just take up space for alignment
						SNew(SBox)
						.WidthOverride(18.0f)
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SHorizontalBox)
						.Visibility(EVisibility::SelfHitTestInvisible)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Log Color:")))
						]
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							// Just take up space for alignment
							SNew(SBox)
							.WidthOverride(4.0f)
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							SAssignNew(m_LogColorPickerWidget, SColorBlock)
							.Color(this, &STestNavmeshWidget::OnGetLogColorForColorBlock)
							.ShowBackgroundForAlpha(true)
							.IgnoreAlpha(false)
							.OnMouseButtonDown(this, &STestNavmeshWidget::OnMouseButtonDownLogColorBlock)
							.Size(FVector2D(18.0f, 18.0f))
						]
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						// Just take up space for alignment
						SNew(SBox)
						.WidthOverride(18.0f)
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						// Just take up space for alignment
						SNew(SButton)
						.TextFlowDirection(ETextFlowDirection::LeftToRight)
						.TextShapingMethod(ETextShapingMethod::FullShaping)
						.Text(FText::FromString(TEXT("Clear Logs")))
						.OnClicked(this, &STestNavmeshWidget::OnClearLogs)
					]

				]
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.Padding(0.0f, 2.0f)
				.FillHeight(1.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FColor::Silver)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(false)
						+ SScrollBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SAssignNew(m_PathTextWidget, STextBlock)
							.AutoWrapText(true)
							.ColorAndOpacity(this, &STestNavmeshWidget::OnGetLogColor)
							.Justification(ETextJustify::Left)
						]

					]
				]

			]

		];

		if (m_BeginEntryBlockWidget.IsValid())
		{
			m_BeginEntryBlockWidget->SetInputLocation(m_BeginLocatin);
		}

		if (m_EndEntryBlockWidget.IsValid())
		{
			m_EndEntryBlockWidget->SetInputLocation(m_EndLocation);
		}

		if (ShowLevelCheckBox.IsValid())
		{
			OnShowToLevelCheckStateChanged(ShowLevelCheckBox->GetCheckedState());
		}

		if (UnrealCoordinateCheckBox.IsValid())
		{
			OnUeCoordinateCheckStateChanged(UnrealCoordinateCheckBox->GetCheckedState());
		}
}

void STestNavmeshWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	RefreshDrawWayPoint();
	RefreshWayPoint(InDeltaTime);
	RefreshPrintfWayPoint();
}

void STestNavmeshWidget::OnCopyLocation()
{
	if (m_FocusEntryBlockWidget.IsValid())
	{
		FVector loc = m_FocusEntryBlockWidget->GetInputLocation();
		FString Str = loc.ToString();// FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), loc.X, loc.Y, loc.Z);
		if (!Str.IsEmpty())
			FPlatformApplicationMisc::ClipboardCopy(*Str);
	}
}

void STestNavmeshWidget::OnPasteLocation()
{
	if (m_FocusEntryBlockWidget.IsValid())
	{
		FString PastedText;
		FPlatformApplicationMisc::ClipboardPaste(PastedText);
		FVector Location;
		if (Location.InitFromString(PastedText))
		{
			m_FocusEntryBlockWidget->SetInputLocation(Location);
		}

		if (m_FocusEntryBlockWidget == m_BeginEntryBlockWidget)
		{
			m_BeginLocatin = Location;
		}
		else
		{
			m_EndLocation = Location;
		}
	}
}

TSharedPtr<SWidget> STestNavmeshWidget::BuildContextMenuContent()
{
	TSharedPtr<FUICommandList> UICommandList = FNavmeshToolModule::Get().GetUICommandList();
	if (UICommandList.IsValid())
	{
		FMenuBuilder MenuBuilder(true, UICommandList);
		{
			MenuBuilder.BeginSection("EditLocation", LOCTEXT("Heading", "Modify Location"));
			// Copy
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().Copy);

			// Paste
			MenuBuilder.AddMenuEntry(FNavmeshToolCommands::Get().Paste);

			//MenuBuilder.AddWidget(SNew(SVectorInputBox),FText::FromString(FString(TEXT("Start Point:"))));
			MenuBuilder.EndSection();
		}

		return MenuBuilder.MakeWidget();
	}

	return nullptr;
}

TSharedRef<SWidget> STestNavmeshWidget::BuildChildWidget(TSharedPtr<class SEntryBlock>& pWidget, FString label)
{
	FSlateFontInfo FontInfo = IDetailLayoutBuilder::GetDetailFont();

	TSharedPtr<SWidget> MenuWidget = BuildContextMenuContent();

	TSharedRef<SWidget> widget = SNew(SBorder)
		.BorderBackgroundColor(FLinearColor::Transparent)
		.Visibility(EVisibility::SelfHitTestInvisible)
		[
			SNew(SBox)
			.Visibility(EVisibility::SelfHitTestInvisible)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.HeightOverride(24.0f)
			[
				SAssignNew(pWidget, SEntryBlock)
				.HoveredImage(m_HoveredImageStyle.Get())
				.UnHoveredImage(m_UnHoveredImageStyle.Get())
				.Visibility(EVisibility::Visible)
				.OnClicked(this, &STestNavmeshWidget::OnEntryBlockClicked)
				.PopMenu(MenuWidget)
				.LabelText(FText::FromString(label))
			]
		];

	return widget;
}

FReply STestNavmeshWidget::OnEntryBlockClicked(TSharedRef<SWidget> pWidget)
{
	m_FocusEntryBlockWidget = StaticCastSharedRef<SEntryBlock>(pWidget);

	return FReply::Handled();
}

FReply STestNavmeshWidget::OnExportTestNavmesh()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return FReply::Unhandled();
	}

	TArray<FString> SaveFilePath;
	FDesktopPlatformModule::Get()->SaveFileDialog(NULL,
		TEXT("Choose A Path to Save The TestMesh"),
		TEXT(""),
		World->GetMapName(),
		TEXT("TestMesh File|*.txt"),
		0,
		SaveFilePath
	);

	if (SaveFilePath.Num() != 1)
	{
		return FReply::Unhandled();
	}

	FString ExportStr;
	ExportStr += TEXT("s Tile Mesh\n");// FString::Printf(TEXT(""))
	ExportStr += FString::Printf(TEXT("f %s.obj\n"), *World->GetMapName());
	ExportStr += FString::Printf(TEXT("n %s.nm\n"), *World->GetMapName());
	ExportStr += FString::Printf(TEXT("rc  %f,%f,%f  %f,%f,%f\n"), 
		m_BeginLocatin.X, m_BeginLocatin.Y, m_BeginLocatin.Z, m_EndLocation.X, m_EndLocation.Y, m_EndLocation.Z);

	FArchive* FileAr = IFileManager::Get().CreateDebugFileWriter(*SaveFilePath[0]);
	if (FileAr != NULL)
	{
		auto AnsiLineToSave = StringCast<ANSICHAR>(*ExportStr);
		FileAr->Serialize((ANSICHAR*)AnsiLineToSave.Get(), AnsiLineToSave.Length());
		FileAr->Close();
	}
	return FReply::Handled();
}

FReply STestNavmeshWidget::OnFindPath()
{
	if (!m_BeginEntryBlockWidget.IsValid() || !m_EndEntryBlockWidget.IsValid())
	{
		return FReply::Unhandled();
	}

	FVector BeginLocation = m_BeginEntryBlockWidget->GetInputLocation();
	FVector EndLocation = m_EndEntryBlockWidget->GetInputLocation();

	UWorld* World = GEditor->GetEditorWorldContext().World();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
		return FReply::Unhandled();

	ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
	if (!NavData)
		return FReply::Unhandled();

	FSharedConstNavQueryFilter NavQueryFilter = NavData->GetDefaultQueryFilter();

	FPathFindingQuery Query(nullptr, *NavData, BeginLocation, EndLocation, NavQueryFilter);
	FPathFindingResult Result = NavSys->FindPathSync(Query);
	if (Result.IsSuccessful())
	{
		
		for (auto it : m_TRenderTextArray)
		{
			World->DestroyActor(it);
		}

		m_TRenderTextArray.Empty();

		m_TPathArray.Empty();
		m_TPathArray = Result.Path->GetPathPoints();

		if (m_TPathArray.Num() > 0)
		{
			InitSpawnWaypoint();
			m_bRefreshPrintfWayPoint = true;
		}

	}

	return FReply::Handled();
}


FReply STestNavmeshWidget::OnClearPath()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		for (auto Actor : m_TRenderTextArray)
		{
			World->DestroyActor(Actor);
		}

		m_TRenderTextArray.Empty();
	}

	m_TPathArray.Empty();

	m_bRefreshPrintfWayPoint = false;

	return FReply::Handled();
}

FLinearColor STestNavmeshWidget::OnGetPathColorForColorBlock() const
{
	return m_PathPickerColor;
}

FReply STestNavmeshWidget::OnMouseButtonDownPathColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled();
	}

	FLinearColor InitialColor = m_PathPickerColor;
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnMouseUp = false;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.sRGBOverride = true;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &STestNavmeshWidget::OnSetPathColorFromColorPicker);
		PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateSP(this, &STestNavmeshWidget::OnPathColorPickerCancelled);
		PickerArgs.OnInteractivePickBegin = FSimpleDelegate::CreateSP(this, &STestNavmeshWidget::OnColorPickerInteractiveBegin);
		PickerArgs.OnInteractivePickEnd = FSimpleDelegate::CreateSP(this, &STestNavmeshWidget::OnColorPickerInteractiveEnd);
		PickerArgs.InitialColorOverride = InitialColor;
		PickerArgs.ParentWidget = m_PathColorPickerWidget;
		PickerArgs.OptionalOwningDetailsView = m_PathColorPickerWidget;
		FWidgetPath ParentWidgetPath;
		if (FSlateApplication::Get().FindPathToWidget(m_PathColorPickerWidget.ToSharedRef(), ParentWidgetPath))
		{
			PickerArgs.bOpenAsMenu = FSlateApplication::Get().FindMenuInWidgetPath(ParentWidgetPath).IsValid();
		}
	}

	OpenColorPicker(PickerArgs);

	return FReply::Handled();
}

void STestNavmeshWidget::OnSetPathColorFromColorPicker(FLinearColor NewColor)
{
	m_PathPickerColor = NewColor;
}

void STestNavmeshWidget::OnPathColorPickerCancelled(FLinearColor OriginalColor)
{
	m_PathPickerColor = OriginalColor;
}

FLinearColor STestNavmeshWidget::OnGetLogColorForColorBlock() const
{
	return m_LogPickerColor;
}

FReply STestNavmeshWidget::OnMouseButtonDownLogColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled();
	}

	FLinearColor InitialColor = m_LogPickerColor;
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnMouseUp = false;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.sRGBOverride = true;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &STestNavmeshWidget::OnSetLogColorFromColorPicker);
		PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateSP(this, &STestNavmeshWidget::OnLogColorPickerCancelled);
		PickerArgs.OnInteractivePickBegin = FSimpleDelegate::CreateSP(this, &STestNavmeshWidget::OnColorPickerInteractiveBegin);
		PickerArgs.OnInteractivePickEnd = FSimpleDelegate::CreateSP(this, &STestNavmeshWidget::OnColorPickerInteractiveEnd);
		PickerArgs.InitialColorOverride = InitialColor;
		PickerArgs.ParentWidget = m_LogColorPickerWidget;
		PickerArgs.OptionalOwningDetailsView = m_LogColorPickerWidget;
		FWidgetPath ParentWidgetPath;
		if (FSlateApplication::Get().FindPathToWidget(m_LogColorPickerWidget.ToSharedRef(), ParentWidgetPath))
		{
			PickerArgs.bOpenAsMenu = FSlateApplication::Get().FindMenuInWidgetPath(ParentWidgetPath).IsValid();
		}
	}

	OpenColorPicker(PickerArgs);

	return FReply::Handled();
}

void STestNavmeshWidget::OnSetLogColorFromColorPicker(FLinearColor NewColor)
{
	m_LogPickerColor = NewColor;
}

void STestNavmeshWidget::OnLogColorPickerCancelled(FLinearColor OriginalColor)
{
	m_LogPickerColor = OriginalColor;
}

FSlateColor STestNavmeshWidget::OnGetLogColor() const
{
	return m_LogPickerColor;
}

void STestNavmeshWidget::OnColorPickerInteractiveBegin()
{
	FString DisplayName = TEXT("Picker Color");
	FText text = FText::Format(LOCTEXT("SetColorProperty", "Edit {0}"), FText::FromString(DisplayName));
	GEditor->BeginTransaction(text);
}

void STestNavmeshWidget::OnColorPickerInteractiveEnd()
{
	GEditor->EndTransaction();
}

void STestNavmeshWidget::SetLog(FString inText)
{
	if (!m_PathTextWidget.IsValid() || inText.IsEmpty())
		return;

	FString CurrentText;
	FString OldText = m_PathTextWidget->GetText().ToString();

	if (!OldText.IsEmpty())
	{
		if (OldText.EndsWith(TEXT("\n")))
		{
			CurrentText = OldText;
		}
		else
		{
			CurrentText = OldText + TEXT("\n");
		}
	}
	else
	{
		CurrentText = TEXT("");
	}

	FString NewText;
	if (inText.EndsWith(TEXT("\n")))
	{
		NewText = CurrentText + inText;
	}
	else
	{
		NewText = CurrentText + inText + TEXT("\n");
	}

	m_PathTextWidget->SetText(NewText);
}

FReply STestNavmeshWidget::OnClearLogs()
{
	m_PathTextWidget->SetText(FString(""));
	return FReply::Handled();
}

void STestNavmeshWidget::OnShowToLevelCheckStateChanged(ECheckBoxState State)
{
	m_bShowToLevel = State == ECheckBoxState::Checked ? true : false;
	if (!m_bShowToLevel)
	{
		ClearWaypoint();
	}
	else
	{
		ResetWaypoint();
	}
}

void STestNavmeshWidget::OnUeCoordinateCheckStateChanged(ECheckBoxState State)
{
	m_bShowUnrealPos = State == ECheckBoxState::Checked ? true : false;
	m_ShowUnrealWidgetName = m_bShowUnrealPos ? TEXT("UnrealCoordinate") : TEXT("RecastCoordinate");
	if (m_UCCheckBoxTextBlock.IsValid()) m_UCCheckBoxTextBlock->SetText(m_ShowUnrealWidgetName);

	RefreshWayPointLable();
	m_bRefreshPrintfWayPoint = true;
}

void STestNavmeshWidget::InitSpawnWaypoint()
{
	if (!m_bShowToLevel || m_TPathArray.Num() <= 0 || m_TRenderTextArray.Num() > 0)
		return;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return;

	for (auto path : m_TPathArray)
	{
		ATextRenderActor* pText = World->SpawnActor<ATextRenderActor>(path.Location, FRotator::ZeroRotator);
		if (!pText) continue;
		FVector loc = m_bShowUnrealPos ? path.Location : Unreal2RecastPoint(path.Location);
		pText->GetTextRender()->SetText(loc.ToText());
		pText->GetTextRender()->SetTextRenderColor(m_PathPickerColor.ToRGBE());
		pText->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
		pText->GetTextRender()->SetVerticalAlignment(EVRTA_TextBottom);
		pText->GetTextRender()->SetVertSpacingAdjust(50.f);
		pText->GetTextRender()->SetWorldSize(50.f);
		m_TRenderTextArray.Add(pText);
	}
}

void STestNavmeshWidget::RefreshWayPointLable()
{
	if (!m_bShowToLevel || m_TRenderTextArray.Num() <= 0)
		return;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return;

	for (auto text : m_TRenderTextArray)
	{
		FVector TextVector;
		TextVector.InitFromString(text->GetTextRender()->Text.ToString());
		FVector loc = m_bShowUnrealPos ? Recast2UnrealPoint(TextVector) : Unreal2RecastPoint(TextVector);
		text->GetTextRender()->SetText(loc.ToText());
		text->GetTextRender()->SetTextRenderColor(m_PathPickerColor.ToRGBE());
	}
}

void STestNavmeshWidget::RefreshWayPoint(const float InDeltaTime)
{
	if (!m_bShowToLevel || m_TRenderTextArray.Num() <= 0)
		return;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return;

	FVector ViewCameraLocation = GetViewportLocation(ELevelViewportType::LVT_Perspective);

	for (auto text : m_TRenderTextArray)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(text->GetTextRender()->RelativeLocation, ViewCameraLocation);
		FRotator CurRotator = text->GetTextRender()->RelativeRotation;
		FRotator NewRotator = UKismetMathLibrary::RInterpTo(CurRotator, rot, InDeltaTime, 5.0f);
		text->GetTextRender()->SetRelativeRotation(NewRotator);
		text->GetTextRender()->SetTextRenderColor(m_PathPickerColor.ToRGBE());
	}
}

void STestNavmeshWidget::RefreshDrawWayPoint()
{
	if (!m_bShowToLevel || m_TPathArray.Num() <= 0)
		return;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return;

	for (int i = 0, j = i + 1; i < m_TPathArray.Num() && j < m_TPathArray.Num(); i++, j++)
	{
		UKismetSystemLibrary::DrawDebugPoint(World, m_TPathArray[i].Location, 10.0f, FLinearColor::Red);
		if (j + 1 == m_TPathArray.Num())
			UKismetSystemLibrary::DrawDebugPoint(World, m_TPathArray[j].Location, 10.0f, FLinearColor::Red);

		float len = FVector::Distance(m_TPathArray[i].Location, m_TPathArray[j].Location);
		UKismetSystemLibrary::DrawDebugArrow(World, m_TPathArray[i].Location, m_TPathArray[j].Location, len, m_PathPickerColor);
	}
}

void STestNavmeshWidget::RefreshPrintfWayPoint()
{
	if (!m_bRefreshPrintfWayPoint || m_TPathArray.Num() <= 0)
		return;

	FString PathText; int32 idx = 0;
	SetLog(m_ShowUnrealWidgetName + TEXT(":"));
	for (auto path : m_TPathArray)
	{
		++idx;
		FVector loc = m_bShowUnrealPos ? path.Location : Unreal2RecastPoint(path.Location);
		PathText += FString::Printf(TEXT("Paths.Point_%d : ( X=%f, Y=%f, Z=%f )\n"), idx, loc.X, loc.Y, loc.Z);

	}
	SetLog(PathText);

	m_bRefreshPrintfWayPoint = false;
}

FVector STestNavmeshWidget::GetViewportLocation(ELevelViewportType ViewportType)
{
	FEditorViewportClient* pCurrentViewport = nullptr;
	for (FEditorViewportClient* CurrentViewport : GEditor->GetAllViewportClients())
	{
		if (CurrentViewport && CurrentViewport->GetViewportType() == ViewportType)
		{
			pCurrentViewport = CurrentViewport;
			break;
		}
	}

	if (!pCurrentViewport)
		return FVector::ZeroVector;

	FVector LookAtLocation = pCurrentViewport->GetViewLocation();
	return LookAtLocation;
}

void STestNavmeshWidget::ResetWaypoint()
{
	InitSpawnWaypoint();
}

void STestNavmeshWidget::ClearWaypoint()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		for (auto Actor : m_TRenderTextArray)
		{
			World->DestroyActor(Actor);
		}

		m_TRenderTextArray.Empty();
	}
}

#undef LOCTEXT_NAMESPACE
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


