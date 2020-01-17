#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"

/************************************************************************
* Desc 	: RoleTypeTable ���ݽṹ
* Author	: WangPeng
* Time		: [4/22/2019 wp]
************************************************************************/
typedef struct tagRoleTypeData
{
	int32                   ID;				        // ID
	FString                 FileName;		        // ��Ӧ��ģ�Ͳ����ļ�����
	FString                 BlueprintName;          // ��ͼ����
	FString                 SkeletonName;		    // ������
	FString                 AnimInstanceName;	    // ����ʵ��
	int32                   AnimationMode;          // ����ģʽ(0:BP����״̬����1��������Դ��2�����ɶ���)EAnimationMode::Type
	float                   Scale;			        // ����
	TArray<FMaterialData>   Material;		        // ���ʣ�Ԥ����
	FString					NormalHurtMontage;
	FString					DiaupHurtMontage;
	FString					BigHurtMontage;
	FString					DeadMontage;

}FRoleTypeData;

/************************************************************************
* Desc 	: FRolePartMeshTable ���ݽṹ
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
* Desc 	: FExtraRolePartMeshTable ���ݽṹ
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
