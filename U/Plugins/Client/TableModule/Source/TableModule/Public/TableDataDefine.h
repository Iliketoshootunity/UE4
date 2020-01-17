#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"

/************************************************************************
* Desc 	: RoleTypeTable 数据结构
* Author	: WangPeng
* Time		: [4/22/2019 wp]
************************************************************************/
typedef struct tagRoleTypeData
{
	int32                   ID;				        // ID
	FString                 FileName;		        // 对应的模型部件文件夹名
	FString                 BlueprintName;          // 蓝图包名
	FString                 SkeletonName;		    // 骨骼名
	FString                 AnimInstanceName;	    // 动画实例
	int32                   AnimationMode;          // 动画模式(0:BP动画状态机，1：动画资源，2：自由动画)EAnimationMode::Type
	float                   Scale;			        // 缩放
	TArray<FMaterialData>   Material;		        // 材质（预留）
	FString					NormalHurtMontage;
	FString					DiaupHurtMontage;
	FString					BigHurtMontage;
	FString					DeadMontage;

}FRoleTypeData;

/************************************************************************
* Desc 	: FRolePartMeshTable 数据结构
* Author	: WangPeng
* Time		: [4/22/2019 wp]
************************************************************************/
typedef struct tagRolePartMeshData
{
	int32 ID;
	FString SkeletalMesh;
	TArray<FMaterialData> MeshMaterial;
}FRolePartMeshData;

/************************************************************************
* Desc 	: FExtraRolePartMeshTable 数据结构
* Author	: WangPeng
* Time		: [4/22/2019 wp]
************************************************************************/
typedef struct tagExtraRolePartMeshData
{
	int32					ID;
	FString					SkeletalMesh;
	FString					BoneName;
	TArray<FMaterialData>	MeshMaterial;
	FVector					OffsetLocation;
	FRotator				Rotator;
	float					Scale;
}FExtraRolePartMeshData;
