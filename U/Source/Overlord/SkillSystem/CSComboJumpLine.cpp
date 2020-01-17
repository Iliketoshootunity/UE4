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

	//����ID,��ȡĿ��ComboƬ�ε���Ϣ
	const FComboClipTableData* TempData = ComboClipTable::Get()->GetData(Data.ID);
	if (TempData)
	{
		ClipData = TempData;
	}
	else
	{
		FBasicFunction::Log("UComboJumpLine::Init() target Clip ID is error in MontageEditor", true);
	}

	//����������������� ����������ѭ��״̬�������κβ������Զ���ʼ��ʱ��ʱ�䵽����Ȼ������һ���ͷŶ���
	if (Data.RespondType == EComboJumLineRespondType::Chanting)
	{
		float time = 0.25f;		//����
	}

}

void UCSComboJumpLine::RespondInput(EInputStatus InputStatus)
{
	CurInputStatus = InputStatus;
	if (OwnClip)
	{
		//��һ���������ͨ��ת�� ����/̧�� ����,���ظ�����,ֱ��ƥ�������ȷλ��
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
		//�ڶ��������������ת������/�������£���ʼ��ʱ,̧�𣬽�����ʱ�������ظ�����
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
			//�������������������֮ǰ����ת����
			if (InputStatus == EInputStatus::Relese)
			{
				OwnClip->MatchInputFailed(this);
			}
		}
		else if (Data.RespondType == EComboJumLineRespondType::Time)
		{
			//���������,�������¼�ʱ�����ظ�����
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
					//�Ƿ���ƥ���Զ��ͷţ���������δ��Ӧ��Ӧ��
					if (ClipData->bMatchChargeRelease != 0 || CurInputStatus == EInputStatus::None)
					{
						//ƥ��ɹ������ͷ�
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
