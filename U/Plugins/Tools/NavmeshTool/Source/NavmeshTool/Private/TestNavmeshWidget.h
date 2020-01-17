// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"
#include <Kismet/KismetStringLibrary.h>
/**
 * 
 */

class STestNavmeshWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STestNavmeshWidget)
	{}
	SLATE_END_ARGS()

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
private:
	/*Menu Function*/
	void					OnCopyLocation();
	void					OnPasteLocation();
	TSharedPtr<SWidget>		BuildContextMenuContent();

	/*Child Widget Function*/
	TSharedRef<SWidget>		BuildChildWidget(TSharedPtr<class SEntryBlock>& pWidget, FString label);
	FReply					OnEntryBlockClicked(TSharedRef<SWidget> pWidget);

	/*Path function*/
	FReply					OnExportTestNavmesh();
	FReply					OnFindPath();
	FReply					OnClearPath();

	/**#PathColorFunction**/
	FLinearColor			OnGetPathColorForColorBlock() const;
	FReply					OnMouseButtonDownPathColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void					OnSetPathColorFromColorPicker(FLinearColor NewColor);
	void					OnPathColorPickerCancelled(FLinearColor OriginalColor);

	/**#LogColorFunction**/
	FLinearColor			OnGetLogColorForColorBlock() const;
	FReply					OnMouseButtonDownLogColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void					OnSetLogColorFromColorPicker(FLinearColor NewColor);
	void					OnLogColorPickerCancelled(FLinearColor OriginalColor);
	FSlateColor				OnGetLogColor() const;

	void					OnColorPickerInteractiveBegin();
	void					OnColorPickerInteractiveEnd();

	/*Log Function*/
	void					SetLog(FString inText);
	FReply					OnClearLogs();

	/*CheckStateChanged Function*/
	void					OnShowToLevelCheckStateChanged(ECheckBoxState State);
	void					OnUeCoordinateCheckStateChanged(ECheckBoxState State);

	/*Tick Logic Function*/
	void					InitSpawnWaypoint();
	void					RefreshWayPointLable();
	void					RefreshWayPoint(const float InDeltaTime);
	void					RefreshDrawWayPoint();
	void					RefreshPrintfWayPoint();
	FVector					GetViewportLocation(ELevelViewportType ViewportType);

	void					ResetWaypoint();
	void					ClearWaypoint();
private:
	static bool								m_bRefreshPrintfWayPoint;
	uint8									m_bShowToLevel : 1;
	uint8									m_bShowUnrealPos : 1;
	static FString							m_ShowUnrealWidgetName;
	TSharedPtr<class STextBlock>			m_UCCheckBoxTextBlock;

	TSharedPtr<struct FSlateBrush>			m_HoveredImageStyle;
	TSharedPtr<struct FSlateBrush>			m_UnHoveredImageStyle;
	TSharedPtr<class SEntryBlock>			m_FocusEntryBlockWidget;
	TSharedPtr<class SEntryBlock>			m_BeginEntryBlockWidget;
	TSharedPtr<class SEntryBlock>			m_EndEntryBlockWidget;
	TSharedPtr<class STextBlock>			m_PathTextWidget;

	static FVector							m_BeginLocatin;
	static FVector							m_EndLocation;
	static TArray<struct FNavPathPoint>		m_TPathArray;
	static TArray<class ATextRenderActor*>	m_TRenderTextArray;

	TSharedPtr<class SColorBlock>			m_PathColorPickerWidget;
	static FLinearColor						m_PathPickerColor;

	TSharedPtr<class SColorBlock>			m_LogColorPickerWidget;
	static FLinearColor						m_LogPickerColor;

};
