// Fill out your copyright notice in the Description page of Project Settings.

#include "RolePartMeshManager.h"
#include "BodyPartTable.h"
#include "BaseConfig.h"
#include "RoleTypeTable.h"
#include "ExtraRolePartMeshTable.h"
#include "RolePartMeshTable.h"
#include "BasicFunction.h"

FRolePartMeshManager::FRolePartMeshManager()
{
}

FRolePartMeshManager::~FRolePartMeshManager()
{
	ResetData();
}

bool FRolePartMeshManager::LoadTable()
{
	if (!BodyPartTable::Get()->IsLoaded()) return false;
	if (!RoleTypeTable::Get()->IsLoaded()) return false;

	ResetData();
	BodyPartTable::FDataMap* pBodyPartData = BodyPartTable::Get()->GetData();
	TArray<FBodyPartFileData> RolePartFileNameArray = RoleTypeTable::Get()->GetBodyPartFileNames();

	for (auto PartFileIt : RolePartFileNameArray)
	{
		//如果存在则不需要再加载
		if (IsExistData(PartFileIt.BodyPartFileName))
		{
			m_RoleType2PartFileNameMap.Add(PartFileIt.RoleTypeID, PartFileIt.BodyPartFileName);
			continue;
		}

		FNpcResData* pData = new FNpcResData();
		for (BodyPartTable::FDataMap::TIterator It(*pBodyPartData); It; ++It)
		{
			BodyPartData& _BodyPartData = It.Value();
			FString BodyPartPath = SBaseConfig::Get()->GetData()->NpcResPath + PartFileIt.BodyPartFileName + FString(TEXT("/")) + _BodyPartData.PartName + FString(TEXT(".txt"));
			if (_BodyPartData.IsExtra)
			{
				FExtraRolePartMeshTable* pTable = new FExtraRolePartMeshTable();
				if (!pTable->LoadTable(BodyPartPath))
				{
					FBasicFunction::Logs(TEXT("%s Is Read Failed!!"), *BodyPartPath);
					return false;
				}
				pData->m_ExtraBodyPartMeshMap.Add(_BodyPartData.BodyPartIndex, pTable);
			}
			else
			{
				FRolePartMeshTable* pTable = new FRolePartMeshTable();
				if (!pTable->LoadTable(BodyPartPath))
				{
					FBasicFunction::Logs(TEXT("%s Is Read Failed!!"), *BodyPartPath);
					return false;
				}
				pData->m_BodyPartMeshMap.Add(_BodyPartData.BodyPartIndex, pTable);
			}
		}

		m_NpcResDataMap.Add(PartFileIt.BodyPartFileName, pData);
		m_RoleType2PartFileNameMap.Add(PartFileIt.RoleTypeID, PartFileIt.BodyPartFileName);
	}
	return true;
}


const FRolePartMeshData* FRolePartMeshManager::GetBodypartData(int32 inRoeTypeID, int32 eBodyPart, int32 nPartID)
{
	if (!m_RoleType2PartFileNameMap.Contains(inRoeTypeID))
		return nullptr;

	FString BodypartName = m_RoleType2PartFileNameMap.FindRef(inRoeTypeID);
	if (BodypartName.IsEmpty())
		return nullptr;

	if (!m_NpcResDataMap.Contains(BodypartName))
		return nullptr;

	FNpcResData* pNpcResData = m_NpcResDataMap.FindRef(BodypartName);
	if (pNpcResData == nullptr)
		return nullptr;

	if (!pNpcResData->m_BodyPartMeshMap.Contains(eBodyPart))
		return nullptr;

	FRolePartMeshTable* pTable = pNpcResData->m_BodyPartMeshMap.FindRef(eBodyPart);
	if (pTable == nullptr)
		return nullptr;

	return pTable->GetData(nPartID);

}

const FExtraRolePartMeshData* FRolePartMeshManager::GetExtraPartData(int32 inRoeTypeID, int32 eBodyPart, int32 nPartID)
{
	if (!m_RoleType2PartFileNameMap.Contains(inRoeTypeID))
		return nullptr;

	FString BodypartName = m_RoleType2PartFileNameMap.FindRef(inRoeTypeID);
	if (BodypartName.IsEmpty())
		return nullptr;

	if (!m_NpcResDataMap.Contains(BodypartName))
		return nullptr;

	FNpcResData* pNpcResData = m_NpcResDataMap.FindRef(BodypartName);
	if (pNpcResData == nullptr)
		return nullptr;

	if (!pNpcResData->m_ExtraBodyPartMeshMap.Contains(eBodyPart))
		return nullptr;

	FExtraRolePartMeshTable* pTable = pNpcResData->m_ExtraBodyPartMeshMap.FindRef(eBodyPart);
	if (pTable == nullptr)
		return nullptr;

	return pTable->GetData(nPartID);
}

bool FRolePartMeshManager::IsExistData(FString PartFileName)
{
	for (auto It : m_RoleType2PartFileNameMap)
	{
		if (It.Value == PartFileName)
		{
			return true;
		}	
	}

	return false;
}

void FRolePartMeshManager::ResetData()
{
	for (TNpcResDataMap::TIterator It(m_NpcResDataMap); It; ++It)
	{
		delete It.Value();
		It.RemoveCurrent();
	}

	m_NpcResDataMap.Empty();
	m_RoleType2PartFileNameMap.Empty();
}
