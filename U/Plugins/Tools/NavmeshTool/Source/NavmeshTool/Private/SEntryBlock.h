// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "SlateDelegates.h"
#include "Optional.h"
/**
 * 
 */
DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnEntryBlockClicked, TSharedRef<SWidget>);

namespace ETransformType
{
	enum Type
	{
		Location,
		Rotation,
		Scale
	};
}

class SEntryBlock : public SBorder
{
public:
	SLATE_BEGIN_ARGS(SEntryBlock)
		: _Content()
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _ContentPadding(FMargin(4.0, 2.0))
		, _Padding(FMargin(0.0, 0.0, 0.0, 0.0))
		, _ClickMethod(EButtonClickMethod::DownAndUp)
		, _IsFocusable(true)
		, _HoveredImage(nullptr)
		, _UnHoveredImage(nullptr)
		, _LabelText()
		, _DesiredSizeScale(FVector2D(1, 1))
		, _ContentScale(FVector2D(1, 1))
		, _ButtonColorAndOpacity(FLinearColor::White)
		, _ForegroundColor(FCoreStyle::Get().GetSlateColor("InvertedForeground"))
		{}

		SLATE_DEFAULT_SLOT(FArguments, Content)
		/** Horizontal alignment */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** Spacing between button's border and the content. */
		SLATE_ATTRIBUTE(FMargin, ContentPadding)

		SLATE_ATTRIBUTE(FMargin, Padding)

		/** Called when the button is clicked */
		SLATE_EVENT(FOnEntryBlockClicked, OnClicked)

		/** Sets the rules to use for determining whether the button was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

		SLATE_ARGUMENT(bool, IsFocusable)
		SLATE_ARGUMENT(const FSlateBrush*, HoveredImage)
		SLATE_ARGUMENT(const FSlateBrush*, UnHoveredImage)

		SLATE_ARGUMENT(TSharedPtr<SWidget>, PopMenu)

		SLATE_ATTRIBUTE(FText, LabelText)

		SLATE_ATTRIBUTE(FVector2D, DesiredSizeScale)

		SLATE_ATTRIBUTE(FVector2D, ContentScale)

		SLATE_ATTRIBUTE(FSlateColor, ButtonColorAndOpacity)

		SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)
	SLATE_END_ARGS()

public:
	void						Construct(const FArguments& InArgs);
	TSharedRef<SWidget>			BuildChildContent(const FArguments& InArgs);
	virtual const FSlateBrush*	GetBorder() const;
public:
	virtual int32				OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool				SupportsKeyboardFocus() const override;
	virtual FReply				OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply				OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void				OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void				OnMouseLeave(const FPointerEvent& MouseEvent) override;

public:
	const FVector				GetInputLocation() { return m_InputLocation; }
	void						SetInputLocation(const FVector loc) { m_InputLocation = loc; }
protected:
	FMargin						GetCombinedPadding() const;
	TEnumAsByte<EButtonClickMethod::Type> GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const;
	void						OnContextMenuClosed(TSharedRef<class IMenu> Menu);

	TOptional<float>			GetLocationX() const { return m_InputLocation.X; }
	TOptional<float>			GetLocationY() const { return m_InputLocation.Y; }
	TOptional<float>			GetLocationZ() const { return m_InputLocation.Z; }

	FVector						GetAxisFilteredVector(EAxisList::Type Axis, const FVector& NewValue, const FVector& OldValue);
	void						OnSetTransformAxis(float NewValue, ETextCommit::Type CommitInfo, ETransformType::Type TransformField, EAxisList::Type Axis, bool bCommitted);
protected:
	uint8 bIsPressed : 1;
	uint8 bIsFocusable : 1;

	TAttribute<FMargin> ContentPadding;
	TAttribute<FMargin> Padding;

	const FSlateBrush* HoveredImage;
	const FSlateBrush* UnHoveredImage;

	/** The delegate to execute when the button is clicked */
	FOnEntryBlockClicked OnClicked;

	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	TSharedPtr<SWidget> PopMenu;
	TSharedPtr<class IMenu> ContextMenu;

	FVector m_InputLocation;
};
