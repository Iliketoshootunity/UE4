// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameMonster.h"
#include "Protoc/map.pb.h"
#include "NpcResTable.h"
#include "NpcsTable.h"

UCSGameMonster::UCSGameMonster()
{

}

void UCSGameMonster::Init(void* Info)
{
	CharacterType = ECharaterType::Monster;
	mapV2::RoundMonsterInfo* MonsterInfo = (mapV2::RoundMonsterInfo*)Info;
	if (MonsterInfo)
	{
		ID = MonsterInfo->lid();
		SetMaxHP(MonsterInfo->maxhp());
		SetHP(MonsterInfo->hp());
		std::string NameStr = MonsterInfo->name();
		FString NameTemp = FString(NameStr.c_str());
		SetCharacterName("Goblin");
		int32 TableId = MonsterInfo->mid();
		//TableId = 2;
		const FNpcsTableData* Data = NpcsTable::Get()->GetData(TableId);
		CharacterTableData = Data;
		if (Data)
		{
			const FRoleTypeData* UnitTypeData = NpcResTable::Get()->GetRoleTypeData(CharacterTableData->RoleTypeID);
			CharacterTypeData = UnitTypeData;
		}
		InitPos = FVector(MonsterInfo->x(), MonsterInfo->y(), 112);
		InitRot = FRotator::ZeroRotator;
	}
}
