// Fill out your copyright notice in the Description page of Project Settings.


#include "SEntryBlock.h"
#include "DrawElements.h"
#include "WidgetPath.h"
#include "SlateApplication.h"
#include "DetailLayoutBuilder.h"
#include "STextBlock.h"
#include "SBox.h"
#include "SVectorInputBox.h"
#include "SlateEnums.h"


void SEntryBlock::Construct(const FArguments& InArgs)
{
	bIsPressed = false;
	m_InputLocation = FVector::ZeroVector;

	bIsFocusable = InArgs._IsFocusable;
	ContentPadding = InArgs._ContentPadding;
	HoveredImage = InArgs._HoveredImage;
	UnHoveredImage = InArgs._UnHoveredImage;
	ClickMethod = InArgs._ClickMethod;
	OnClicked = InArgs._OnClicked;
	Padding = InArgs._Padding;
	PopMenu = InArgs._PopMenu;

	SBorder::Construct(SBorder::FArguments()
		.ContentScale(InArgs._ContentScale)
		.DesiredSizeScale(InArgs._DesiredSizeScale)
		.BorderBackgroundColor(InArgs._ButtonColorAndOpacity)
		.ForegroundColor(InArgs._ForegroundColor)
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		.Padding(TAttribute<FMargin>(this, &SEntryBlock::GetCombinedPadding))
		.BorderImage(this, &SEntryBlock::GetBorder)
		[
			BuildChildContent(InArgs)
		]
	);
}

TSharedRef<SWidget> SEntryBlock::BuildChildContent(const FArguments& InArgs)
{
	FText label = InArgs._LabelText.IsSet() ? InArgs._LabelText.Get() : FText::FromString(TEXT(""));
	FSlateFontInfo FontInfo = IDetailLayoutBuilder::GetDetailFont();

	if ((InArgs._Content.Widget == SNullWidget::NullWidget))
	{
		return SNew(SHorizontalBox)
			.Visibility(EVisibility::SelfHitTestInvisible)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(label)
				.ColorAndOpacity(FLinearColor::White)
				.Visibility(EVisibility::SelfHitTestInvisible)
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
			.MaxWidth(125.f * 3)
			.FillWidth(1)
			[
				SNew(SVectorInputBox)
				.X(this, &SEntryBlock::GetLocationX)
				.Y(this, &SEntryBlock::GetLocationY)
				.Z(this, &SEntryBlock::GetLocationZ)
				.bColorAxisLabels(true)
				.AllowResponsiveLayout(true)
				.OnXCommitted(this, &SEntryBlock::OnSetTransformAxis, ETransformType::Location, EAxisList::X, true)
				.OnYCommitted(this, &SEntryBlock::OnSetTransformAxis, ETransformType::Location, EAxisList::Y, true)
				.OnZCommitted(this, &SEntryBlock::OnSetTransformAxis, ETransformType::Location, EAxisList::Z, true)
				.Font(FontInfo)
				.AllowSpin(false)
				.Visibility(EVisibility::Visible)

			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				// Just take up space for alignment
				SNew(SBox)
				.WidthOverride(18.0f)
			];
	}
	else
	{
		return InArgs._Content.Widget;
	}
	
}

bool SEntryBlock::SupportsKeyboardFocus() const
{
	return bIsFocusable;
}

const FSlateBrush* SEntryBlock::GetBorder() const
{
	if (IsHovered())
	{
		return HoveredImage;
	}
	else
	{
		return UnHoveredImage;
	}
}

int32 SEntryBlock::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	bool bEnabled = ShouldBeEnabled(bParentEnabled);
	bool bShowDisabledEffect = (UnHoveredImage->DrawAs == ESlateBrushDrawType::NoDrawType);

	const FSlateBrush* BrushResource =  GetBorder();

	ESlateDrawEffect DrawEffects = bShowDisabledEffect && !bEnabled ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BrushResource,
			DrawEffects,
			BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BorderBackgroundColor.Get().GetColor(InWidgetStyle)
		);
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled);
}

FReply SEntryBlock::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.GetEffectingButton() == EKeys::RightMouseButton || MouseEvent.IsTouchEvent()))
	{
		bIsPressed = true;
		EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);

		if (InputClickMethod == EButtonClickMethod::MouseDown)
		{
			
			//get the reply from the execute function
			Reply = OnClicked.IsBound() ? OnClicked.Execute( SharedThis(this) ) : FReply::Handled();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else if (InputClickMethod == EButtonClickMethod::PreciseClick)
		{
			// do not capture the pointer for precise taps or clicks
			// 
			Reply = FReply::Handled();
		}
		else
		{
			//we need to capture the mouse for MouseUp events
			Reply = FReply::Handled().CaptureMouse(AsShared());
		}
	}

	Invalidate(EInvalidateWidget::Layout);

	//return the constructed reply
	return Reply;
}

FReply SEntryBlock::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	const EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);
	const bool bMustBePressed = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));

	if (bMeetsPressedRequirements && ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.GetEffectingButton() == EKeys::RightMouseButton || MouseEvent.IsTouchEvent())))
	{
		bIsPressed = false;

		if (IsEnabled())
		{
			if (InputClickMethod == EButtonClickMethod::MouseDown)
			{
				// NOTE: If we're configured to click on mouse-down/precise-tap, then we never capture the mouse thus
				//       may never receive an OnMouseButtonUp() call.  We make sure that our bIsPressed
				//       state is reset by overriding OnMouseLeave().
			}
			else
			{
				bool bEventOverButton = IsHovered();

				if (!bEventOverButton && MouseEvent.IsTouchEvent())
				{
					bEventOverButton = MyGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition());
				}

				if (bEventOverButton)
				{
					// If we asked for a precise tap, all we need is for the user to have not moved their pointer very far.
					const bool bTriggerForTouchEvent = InputClickMethod == EButtonClickMethod::PreciseClick;

					// If we were asked to allow the button to be clicked on mouse up, regardless of whether the user
					// pressed the button down first, then we'll allow the click to proceed without an active capture
					const bool bTriggerForMouseEvent = (InputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

					if ((bTriggerForTouchEvent || bTriggerForMouseEvent) && OnClicked.IsBound() == true)
					{
						Reply = OnClicked.Execute(SharedThis(this));
					}

					if ((bTriggerForTouchEvent || bTriggerForMouseEvent)&& (PopMenu && PopMenu.IsValid()) && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
					{
						FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
						
						static const bool bFocusImmediately = true;
						ContextMenu = FSlateApplication::Get().PushMenu(
							MouseEvent.GetWindow(),
							WidgetPath,
							PopMenu.ToSharedRef(),
							MouseEvent.GetScreenSpacePosition(),
							FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu),
							bFocusImmediately
						);

						// Make sure the window is valid. It's possible for the parent to already be in the destroy queue, for example if the editable text was configured to dismiss it's window during OnTextCommitted.
						if (ContextMenu.IsValid())
						{
							ContextMenu->GetOnMenuDismissed().AddRaw(this, &SEntryBlock::OnContextMenuClosed);
						}
						else
						{
							ContextMenu.Reset();
						}
					}
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}

	//If the user hasn't requested a new mouse captor and the button still has mouse capture,
	//then the default behavior of the button is to release mouse capture.
	if (Reply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		Reply.ReleaseMouseCapture();
	}

	Invalidate(EInvalidateWidget::Layout);

	return Reply;
}

void SEntryBlock::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SBorder::OnMouseEnter(MyGeometry, MouseEvent);


	Invalidate(EInvalidateWidget::Layout);
}

void SEntryBlock::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SWidget::OnMouseLeave(MouseEvent);


	Invalidate(EInvalidateWidget::Layout);
}

FMargin SEntryBlock::GetCombinedPadding() const
{
	return ContentPadding.Get() + Padding.Get();
}

TEnumAsByte<EButtonClickMethod::Type> SEntryBlock::GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const
{
	if (MouseEvent.IsTouchEvent())
	{
		switch (ClickMethod)
		{
		case EButtonTouchMethod::Down:
			return EButtonClickMethod::MouseDown;
		case EButtonTouchMethod::DownAndUp:
			return EButtonClickMethod::DownAndUp;
		case EButtonTouchMethod::PreciseTap:
			return EButtonClickMethod::PreciseClick;
		}
	}

	return ClickMethod;
}

void SEntryBlock::OnContextMenuClosed(TSharedRef<class IMenu> Menu)
{
	FSlateApplication::Get().SetKeyboardFocus(SharedThis(this), EFocusCause::OtherWidgetLostFocus);
}

FVector SEntryBlock::GetAxisFilteredVector(EAxisList::Type Axis, const FVector& NewValue, const FVector& OldValue)
{
	return FVector((Axis & EAxisList::X) ? NewValue.X : OldValue.X,
		(Axis & EAxisList::Y) ? NewValue.Y : OldValue.Y,
		(Axis & EAxisList::Z) ? NewValue.Z : OldValue.Z);
}

void SEntryBlock::OnSetTransformAxis(float NewValue, ETextCommit::Type CommitInfo, ETransformType::Type TransformField, EAxisList::Type Axis, bool bCommitted)
{
	m_InputLocation = GetAxisFilteredVector(Axis, FVector(NewValue), m_InputLocation);
}

