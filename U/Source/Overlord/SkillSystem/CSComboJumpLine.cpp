// Fill out your copyright notice in the Description page of Project Settings.


#include "CSComboJumpLine.h"
#include "CSComboClip.h"
#include "time.h"
#include "ComboClipTable.h"
#include "TimerManager.h"


FString UCSComboJumpLine::GetMontageName()
{
	if (ClipData == nullptr)return "";
	return ClipData->Path;
}

void UCSComboJumpLine::Init(UCSComboClip* OwnClip, FComboJumpLineData NewData)
{
	this->OwnClip = OwnClip;
	this->Data = NewData;

	SetCanJump(true);
	if (Data.RespondType == EComboJumLineRespondType::Charge)
	{
		SetCanJump(false);
		bStartCharge = true;
	}

	//根据ID,获取目标Combo片段的信息
	const FComboClipTableData* TempData = ComboClipTable::Get()->GetData(Data.ID);
	if (TempData)
	{
		ClipData = TempData;
	}
	else
	{
		FBasicFunction::Log("UComboJumpLine::Init() target Clip ID is error in MontageEditor", true);
	}

	//第五种情况，吟唱， 处于吟唱的循环状态，无需任何操作，自动开始计时，时间到了自然播放下一个释放动画
	if (Data.RespondType == EComboJumLineRespondType::Chanting)
	{
		float time = 0.25f;		//测试
	}

}

void UCSComboJumpLine::RespondInput(EInputStatus InputStatus)
{
	CurInputStatus = InputStatus;
	if (OwnClip)
	{
		//第一种情况，普通跳转， 按下/抬起 触发,可重复进入,直到匹配操作正确位置
		if (Data.RespondType == EComboJumLineRespondType::PressImmediate && InputStatus == EInputStatus::Press)
		{
			OwnClip->MatchInputSucceed(this);
			return;
		}
		else if (Data.RespondType == EComboJumLineRespondType::ReleseImmediate && InputStatus == EInputStatus::Relese)
		{
			OwnClip->MatchInputSucceed(this);
			return;
		}
		//第二种情况，蓄力跳转，按下/持续按下，开始计时,抬起，结束计时，不可重复进入
		else if (Data.RespondType == EComboJumLineRespondType::Charge)
		{
			if (InputStatus == EInputStatus::Relese)
			{
				bStartCharge = false;
				SetCanJump(true);
				if (ChargeTimer <= ClipData->ChargeLength)
				{
					if (OwnClip)
					{
						OwnClip->MatchInputFailed(this);
					}
				}

			}
		}
		else if (Data.RespondType == EComboJumLineRespondType::BeforeCharge || Data.RespondType == EComboJumLineRespondType::AutoSuccessful)
		{
			//第三种情况，蓄力动画之前的跳转条件
			if (InputStatus == EInputStatus::Relese)
			{
				OwnClip->MatchInputFailed(this);
			}
		}
		else if (Data.RespondType == EComboJumLineRespondType::Time)
		{
			//第四种情况,持续按下计时，可重复进入
			if (InputStatus == EInputStatus::Press)
			{
				TimeTimer = 0;
				bStartTime = true;
			}
			else if (InputStatus == EInputStatus::Relese)
			{
				TimeTimer = 0;
				bStartTime = false;
			}
		}
	}
}


void UCSComboJumpLine::Reset()
{
	ClipData = nullptr;
	ChargeTimer = 0;
	OwnClip = nullptr;
	bStartCharge = false;
	SetCanJump(true);
}


void UCSComboJumpLine::Tick(float DeltaTime)
{
	if (Data.RespondType == EComboJumLineRespondType::Charge)
	{
		if (bStartCharge)
		{
			ChargeTimer += DeltaTime;
			if (ClipData)
			{
				if (!bMatchCharge && ChargeTimer > ClipData->ChargeLength)
				{
					//是否是匹配自动释放，或者输入未响应响应到
					if (ClipData->bMatchChargeRelease != 0 || CurInputStatus == EInputStatus::None)
					{
						//匹配成功立即释放
						SetCanJump(true);
					}
					bMatchCharge = true;
					OwnClip->MatchInputSucceed(this);
				}
			}
		}
	}
	else if (Data.RespondType == EComboJumLineRespondType::Time)
	{
		if (bStartTime)
		{
			TimeTimer += DeltaTime;
			if (TimeTimer >= 0.2f)
			{
				OwnClip->MatchInputSucceed(this);
			}
		}
	}
	else if (Data.RespondType == EComboJumLineRespondType::Chanting)
	{

	}

}

bool UCSComboJumpLine::IsTickable() const
{
	if (Data.RespondType == EComboJumLineRespondType::Charge
		|| Data.RespondType == EComboJumLineRespondType::Time ||
		Data.RespondType == EComboJumLineRespondType::Chanting)
	{
		if (OwnClip == nullptr)return false;
		return OwnClip->IsContainJumpLines((UCSComboJumpLine*)this);
	}
	return false;
}

TStatId UCSComboJumpLine::GetStatId() const
{
	return Super::GetStatID();
}
