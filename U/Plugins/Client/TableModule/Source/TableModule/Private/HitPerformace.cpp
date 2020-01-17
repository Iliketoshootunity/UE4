
#include "HitPerformace.h"
#include "BasicFunction.h"

HitPerformace::HitPerformace() :STabBaseTable(this)
{

}

HitPerformace::~HitPerformace()
{

}

bool HitPerformace::ReadTable(int32 nRow, int32& nCol)
{
	FHitPerformaceTableData Data;
	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.ID))
		return false;
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.HitType);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.AirHitType);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.ControlType);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.HitChannel);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.bCanHitAir);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, Data.bCanHitGround);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.HardLevelCut);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.RootMontionScale);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.PushDistance);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.AttackerPauseDelayFrame);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.AttackerPauseDurationFrame);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.SuffererPauseDelayFrame);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, Data.SuffererPauseDurationFrame);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitMeatFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitWoodFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitEarthFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitMetalFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitStoneFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.HitFXRotate);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.BlockedFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.BreakBlockedFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.WeaknessHittedFX);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.CameraShake);
	SetData(Data.ID, Data);
	return true;
}

