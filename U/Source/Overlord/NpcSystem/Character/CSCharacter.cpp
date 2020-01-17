// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimMontage.h"
#include "BasicFunction.h"
#include "AIController.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "BaseSystem/FSM/FSMState.h"
#include "BaseSystem/FSM/FSMMachine.h"
#include "GameFramework/SpringArmComponent.h"
#include "SkillSystem/CSSkillDefine.h"
#include "SkillSystem/CSSkillComponent.h"
#include "SkillSystem/CSSkill.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseSystem/Ext/ActorInterpMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseSystem/GameUtil.h"
#include "WidgetComponent.h"
#include "../Other/CSSearchEnemyComponent.h"
#include "../Other/CSHead.h"
#include "../GameCharacter/CSGameCharacter.h"
#include "SkillSystem/CSComboClip.h"
#include "SGameInstance.h"
#include "../Weapon/CSWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "WorldSystem/SWorldManager.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "SkillSystem/AtkShape/CSAtkBox.h"
#include "SkillSystem/SkiiResult/CSSkillResult.h"
#include "../FSM/CSIdleState.h"
#include "../FSM/CSRunState.h"
#include "../FSM/CSRollState.h"
#include "../FSM/CSSkillState.h"
#include "../FSM/CSHurtState.h"
#include "../FSM/CSDeadState.h"
#include "OtherSystem/Notify/CSHitCheck_NS.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACSCharacter::ACSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InterpMovement = CreateDefaultSubobject<UActorInterpMovementComponent>(TEXT("ActorInterpMovement"));
	Skill = CreateDefaultSubobject<UCSSkillComponent>(TEXT("Skill"));

}

// Called when the game starts or when spawned
void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//获取组件
	UClass* CameraBoomClass = USpringArmComponent::StaticClass();
	CameraBoom = Cast<USpringArmComponent>(GetComponentByClass(CameraBoomClass));

	UClass* CameraClass = UCameraComponent::StaticClass();
	CameraComponent = Cast<UCameraComponent>(GetComponentByClass(CameraClass));

	UClass* SearchEnemyClass = UCSSearchEnemyComponent::StaticClass();
	SearchEnemy = Cast<UCSSearchEnemyComponent>(GetComponentByClass(SearchEnemyClass));

	UClass* WidgetClass = UWidgetComponent::StaticClass();
	HeadWidget = Cast<UWidgetComponent>(GetComponentByClass(WidgetClass));

	SkillResult = NewObject<UCSSkillResult>();

	LocalMeshZ = GetPawnMesh()->GetRelativeTransform().GetLocation().Z;

	DynamicMaterial = GetPawnMesh()->CreateDynamicMaterialInstance(0);
}

void ACSCharacter::BeginDestroy()
{
	if (Skill)
	{
		Skill->Destroy();
	}

	Super::BeginDestroy();

}

// Called every frame
void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BasicMachine)
	{
		BasicMachine->GetCurrentState()->Update(DeltaTime);
		if (CharacterInfo->GetCharacterType() == ECharaterType::MainPlayer)
		{
			//FBasicFunction::Log(FString::FromInt(BasicMachine->GetCurrentStateID()), true);
		}
	}
	UpdateInterpMeshLocation(DeltaTime);
}


void ACSCharacter::Init(UCSGameCharacter* InCharacterInfo)
{
	if (InCharacterInfo == nullptr)return;
	CharacterInfo = InCharacterInfo;
	Head = NewObject<UCSHead>(this);
	Head->Init(CharacterInfo);

	//构建运动状态机
	ConstructionMotionMachine();

	ToIdle();
	Skill->Init();

}


void ACSCharacter::InterpMoveToTarget(FVector TargetRot, float Time)
{
	InterpMovement->StartMoveToTarget(TargetRot, Time);
}

void ACSCharacter::StopMove()
{
	InterpMovement->StopMovementImmediately();
	GetCharacterMovement()->StopMovementImmediately();
}


float ACSCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate /* = 1.f */, FName StartSectionName /* = NAME_None */)
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0;
}

void ACSCharacter::PlayMontage(FString MontageName, UAnimMontage*& OutAnimMontag, FOnMontageEnded EndDelegate)
{
	if (OutAnimMontag == nullptr)
	{
		FString MontageFilePath = MontageName;

		UObject* LoadObj = UGameUtil::LoadClass<UObject>(MontageName);
		if (LoadObj)
		{
			OutAnimMontag = Cast<UAnimMontage>(LoadObj);
			if (OutAnimMontag == nullptr)
			{
				FBasicFunction::Logs(TEXT("%s no load montage"), *MontageFilePath, true);
			}
		}
		else
		{
			FBasicFunction::Logs(TEXT("%s no load montage"), *MontageFilePath, true);
		}
	}
	if (OutAnimMontag != nullptr)
	{
		//播放蒙太奇
		PlayAnimMontage(OutAnimMontag, 1, "");
		//绑定结束事件
		if (EndDelegate.IsBound())
		{
			USkeletalMeshComponent* UseMesh = GetPawnMesh();
			if (UseMesh && UseMesh->AnimScriptInstance)
			{
				UseMesh->AnimScriptInstance->Montage_SetEndDelegate(EndDelegate, OutAnimMontag);
			}
		}
	}
	return;
}


void ACSCharacter::StopAnimMontage(UAnimMontage* AnimMontage)
{
	if (AnimMontage == nullptr)return;
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance &&
		UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
	}
}

void ACSCharacter::StopAllAnimMontages(float BlendTime)
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(BlendTime);
	}
}


int32 ACSCharacter::GetAnimationMode()
{
	if (!GetPawnMesh())return -1;

	return GetPawnMesh()->GetAnimationMode();
}

void ACSCharacter::StopAniFrame(float Dealy, float PauseTime)
{
	ResetAniFrame();
	FTimerDelegate TimeCallBack1 = FTimerDelegate::CreateUObject(this, &ACSCharacter::SetAniRateScale, 0.01f);
	GetWorldTimerManager().SetTimer(DelayStopAniHandle, TimeCallBack1, Dealy, false);
	FTimerDelegate TimeCallBack2 = FTimerDelegate::CreateUObject(this, &ACSCharacter::SetAniRateScale, 1.f);
	GetWorldTimerManager().SetTimer(DoStopAniHandle, TimeCallBack2, PauseTime + Dealy, false);
}

void ACSCharacter::ResetAniFrame()
{
	GetWorldTimerManager().ClearTimer(DelayStopAniHandle);
	GetWorldTimerManager().ClearTimer(DoStopAniHandle);
	SetAniRateScale(1);
}

void ACSCharacter::SetAniRateScale(float AniRate)
{
	GetPawnMesh()->GlobalAnimRateScale = AniRate;
}


USkeletalMeshComponent* ACSCharacter::GetPawnMesh() const
{
	return GetMesh();
}

bool ACSCharacter::IsMainPlayer() const
{
	if (CharacterInfo->GetCharacterType() == ECharaterType::MainPlayer)
	{
		return true;
	}
	return false;
}

bool ACSCharacter::IsAlive() const
{
	if (CharacterInfo == nullptr)return false;
	bool Ret = CharacterInfo->GetHp() > 0 ? true : false;
	return Ret;
}


void ACSCharacter::SetSkeletalMesh(USkeletalMesh* NewSkeletalMesh, FVector Scale3D, EAnimationMode::Type AnimMode, FString AnimInstanceName, TArray<FMaterialData> Materials, bool bReinitPose /*= true*/)
{
	if (!GetMesh() || !NewSkeletalMesh) return;

	GetMesh()->SetSkeletalMesh(NewSkeletalMesh, bReinitPose);
	GetMesh()->SetRelativeScale3D(Scale3D);
	GetMesh()->SetAnimationMode(AnimMode);

	UClass* LoadAnimClass = UGameUtil::LoadClass<UClass>(AnimInstanceName);
	if (LoadAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(LoadAnimClass);
	}

	for (auto MaterialData : Materials)
	{
		UMaterialInterface* pMaterial = UGameUtil::LoadClass<UMaterialInterface>(MaterialData.MaterialName);
		if (!pMaterial) continue;
		GetMesh()->SetMaterial(MaterialData.nSlot, pMaterial);
	}
}


void ACSCharacter::StartInterpMeshLocation(FVector From, FVector To, float InterpTime)
{
	GetMesh()->SetWorldLocation(From);
	InterpMeshFrom = FVector(GetPawnMesh()->GetRelativeTransform().GetLocation().X, GetPawnMesh()->GetRelativeTransform().GetLocation().Y, LocalMeshZ);
	InterpMeshTo = FVector(0, 0, LocalMeshZ);
	InterpMeshTime = InterpTime;
	bInterpMeshLocation = true;
	if (InterpMeshTime <= 0)
	{
		InterpMeshTime = 0.001f;
	}
	InterpMeshTimer = 0;
}

void ACSCharacter::StopInterpMeshLocation()
{
	bInterpMeshLocation = false;
}

void ACSCharacter::UpdateInterpMeshLocation(float DelatTime)
{
	if (bInterpMeshLocation)
	{
		bool IsOK = false;
		InterpMeshTimer += DelatTime;
		if (InterpMeshTimer > InterpMeshTime)
		{
			InterpMeshTimer = InterpMeshTime;
			IsOK = true;
		}
		FVector Loc = UKismetMathLibrary::VLerp(InterpMeshFrom, InterpMeshTo, InterpMeshTimer / InterpMeshTime);
		GetMesh()->SetRelativeLocation(Loc);
		if (IsOK)
		{
			bInterpMeshLocation = false;
		}
	}
}

void ACSCharacter::AdjustCameraBoomArmLength(float MinLength, float MaxLength, float Speed)
{
	if (CameraBoom)
	{
		float DeltaSeconds = 0.02f;
		float TargetLength = DeltaSeconds * Speed + CameraBoom->TargetArmLength;
		if (TargetLength < MaxLength)
		{
			if (TargetLength < MinLength)
			{
				CameraBoom->TargetArmLength = MinLength;
			}
			else
			{
				CameraBoom->TargetArmLength = TargetLength;
			}
		}
		else
		{
			CameraBoom->TargetArmLength = MaxLength;
		}
	}
}


void ACSCharacter::MoveToTargetPosition(FVector TargetPostion, float MaxSpeed)
{
	float Distance = UKismetMathLibrary::Vector_Distance(TargetPostion, GetActorLocation());
	if (Distance > 100)
	{
		//立马转向目标
		FRotator NewRotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPostion);
		SetActorRotation(FRotator(GetActorRotation().Pitch, NewRotate.Yaw, GetActorRotation().Roll));
	}
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	AAIController* AC = Cast<AAIController>(GetController());
	if (AC == nullptr)return;
	AC->MoveToLocation(TargetPostion, 0);
}


void ACSCharacter::ConstructionMotionMachine()
{
	// 基础状态机
	BasicMachine = NewObject<UFSMMachine>(this);
	if (BasicMachine != NULL)
	{
		UFSMState* IdleState = NewObject<UCSIdleState>(BasicMachine);
		IdleState->Init((int32)EMotionMachineState::Idle, (int32)EMotionMachineTransition::ToIdle, BasicMachine, this);
		IdleState->AddTransition((int32)EMotionMachineTransition::ToRoll, (int32)EMotionMachineState::Roll);
		IdleState->AddTransition((int32)EMotionMachineTransition::ToRun, (int32)EMotionMachineState::Run);
		IdleState->AddTransition((int32)EMotionMachineTransition::ToSkill, (int32)EMotionMachineState::Skill);
		IdleState->AddTransition((int32)EMotionMachineTransition::ToHurt, (int32)EMotionMachineState::Hurt);
		IdleState->AddTransition((int32)EMotionMachineTransition::ToDead, (int32)EMotionMachineState::Dead);

		UCSRunState* RunState = NewObject<UCSRunState>(BasicMachine);
		RunState->Init((int32)EMotionMachineState::Run, (int32)EMotionMachineTransition::ToRun, BasicMachine, this);
		RunState->AddTransition((int32)EMotionMachineTransition::ToIdle, (int32)EMotionMachineState::Idle);
		RunState->AddTransition((int32)EMotionMachineTransition::ToSkill, (int32)EMotionMachineState::Skill);
		RunState->AddTransition((int32)EMotionMachineTransition::ToRoll, (int32)EMotionMachineState::Roll);
		RunState->AddTransition((int32)EMotionMachineTransition::ToHurt, (int32)EMotionMachineState::Hurt);

		UFSMState* RollState = NewObject<UCSRollState>(BasicMachine);
		RollState->Init((int32)EMotionMachineState::Roll, (int32)EMotionMachineTransition::ToRoll, BasicMachine, this);
		RollState->AddTransition((int32)EMotionMachineTransition::ToIdle, (int32)EMotionMachineState::Idle);
		RollState->AddTransition((int32)EMotionMachineTransition::ToHurt, (int32)EMotionMachineState::Hurt);

		UFSMState* SkillState = NewObject<UCSSkillState>(BasicMachine);
		SkillState->Init((int32)EMotionMachineState::Skill, (int32)EMotionMachineTransition::ToSkill, BasicMachine, this);
		SkillState->AddTransition((int32)EMotionMachineTransition::ToIdle, (int32)EMotionMachineState::Idle);
		SkillState->AddTransition((int32)EMotionMachineTransition::ToRoll, (int32)EMotionMachineState::Roll);
		SkillState->AddTransition((int32)EMotionMachineTransition::ToHurt, (int32)EMotionMachineState::Hurt);

		UFSMState* HurtState = NewObject<UCSHurtState>(BasicMachine);
		HurtState->Init((int32)EMotionMachineState::Hurt, (int32)EMotionMachineTransition::ToHurt, BasicMachine, this);
		HurtState->AddTransition((int32)EMotionMachineTransition::ToIdle, (int32)EMotionMachineState::Idle);
		HurtState->AddTransition((int32)EMotionMachineTransition::ToDead, (int32)EMotionMachineState::Dead);

		UFSMState* DeadState = NewObject<UCSDeadState>(BasicMachine);
		DeadState->Init((int32)EMotionMachineState::Dead, (int32)EMotionMachineTransition::ToDead, BasicMachine, this);

		BasicMachine->AddState(IdleState);
		BasicMachine->AddState(RollState);
		BasicMachine->AddState(RunState);
		BasicMachine->AddState(SkillState);
		BasicMachine->AddState(HurtState);
		BasicMachine->AddState(DeadState);

	}

	//空间状态机
	SpaceMachine = NewObject<UFSMMachine>(this);
	if (SpaceMachine)
	{
		UFSMState* GroundState = NewObject<UFSMState>(BasicMachine);
		GroundState->Init((int32)ESpaceMachineState::Ground, (int32)ESpaceMachineTransition::ToGround, SpaceMachine, this);
		GroundState->AddTransition((int32)ESpaceMachineTransition::ToAir, (int32)ESpaceMachineState::Air);

		UFSMState* AirState = NewObject<UFSMState>(BasicMachine);
		AirState->Init((int32)ESpaceMachineState::Air, (int32)ESpaceMachineTransition::ToAir, SpaceMachine, this);
		AirState->AddTransition((int32)ESpaceMachineTransition::ToGround, (int32)ESpaceMachineState::Air);

		SpaceMachine->AddState(GroundState);
		SpaceMachine->AddState(AirState);
	}

}

void ACSCharacter::ToIdle()
{
	if (BasicMachine->GetCurrentStateID() == (int32)EMotionMachineState::Idle)
	{
		return;
	}
	BasicMachine->ChangeState((int32)EMotionMachineTransition::ToIdle);
}

void ACSCharacter::ToRun(FVector RunTarget, float MoveSpeed)
{
	UCSRunState* RunState = BasicMachine->GetState<UCSRunState>((int32)EMotionMachineState::Run);
	RunState->SetMoveSpeed(MoveSpeed);
	RunState->SetRunTarget(RunTarget);
	BasicMachine->ChangeState((int32)EMotionMachineTransition::ToRun);
}

void ACSCharacter::ToHurt(FString HurtPath)
{
	UCSHurtState* HurtState = BasicMachine->GetState<UCSHurtState>((int32)EMotionMachineState::Hurt);
	HurtState->SetHurtPath(HurtPath);
	BasicMachine->ChangeState((int32)EMotionMachineTransition::ToHurt);
}

void ACSCharacter::ToDead(ACSCharacter* Attacker)
{
	if (BasicMachine->ChangeState((int32)EMotionMachineTransition::ToDead))
	{
		//打断技能
		if (Skill)
		{
			Skill->CurSkillFinished();
		}
	}
}

bool ACSCharacter::ToSkill(FString ComboPath)
{
	UCSIdleState* IdleState = BasicMachine->GetState<UCSIdleState>((int32)EMotionMachineState::Idle);
	if (IdleState)
	{
		IdleState->SetFightIdle(true);
	}
	UCSSkillState* SkillState = BasicMachine->GetState<UCSSkillState>((int32)EMotionMachineState::Skill);
	SkillState->SetComboPath(ComboPath);
	BasicMachine->ChangeState((int32)EMotionMachineTransition::ToSkill);
	return true;
}

void ACSCharacter::ToRoll()
{
	if (bCanRoll)
	{
		UCSRollState* RollState = BasicMachine->GetState<UCSRollState>((int32)EMotionMachineState::Roll);
		RollState->SetRollPath("/Game/Character/Hero/Anim/Montage/Roll.Roll");
		BasicMachine->ChangeState((int32)EMotionMachineTransition::ToRoll);
	}
}
void ACSCharacter::ForcibleToIdle()
{
	SetCanRoll(true);
	StopMove();
	UFSMState* CurState = BasicMachine->GetCurrentState();
	CurState->Exit();
	BasicMachine->ForcibleChangeState((int32)EMotionMachineState::Idle);
}

bool ACSCharacter::CanReleseSkill()
{
	bool IsOk = BasicMachine->CanChange((int32)EMotionMachineTransition::ToSkill);
	return IsOk;
}


void ACSCharacter::PrepareComboClip()
{
	FComboJumpLineData PreLineData = Skill->GetCurSkill()->GetCurComboClip()->GetPreLineData();
	GetCharacterMovement()->SetMovementMode(PreLineData.MovementMode);
}


bool ACSCharacter::GetIsFightIdle()
{
	if (BasicMachine == nullptr)return false;
	UCSIdleState* IdleState = BasicMachine->GetState<UCSIdleState>((int32)EMotionMachineState::Idle);
	if (IdleState)
	{
		return IdleState->IsFightIdle();
	}
	return false;
}

bool ACSCharacter::GetIsSprint()
{
	if (BasicMachine == nullptr)return false;
	UCSRunState* RunState = BasicMachine->GetState<UCSRunState>((int32)EMotionMachineState::Run);
	if (RunState)
	{
		return RunState->GetIsSprint();
	}
	return false;
}

void ACSCharacter::ReceiverSkill_Implementation(FSkillReceiveInfo ReceiveInfo)
{
	//技能处理逻辑
	SkillResult->LocalInit(ReceiveInfo);
	//ToHurt(SkillResult->GetHurtClipName());
}

void ACSCharacter::ReleaseSkillData()
{
	if (Skill)
	{
		Skill->CurSkillFinished();
	}
}

void ACSCharacter::CreateAtkShape(FHitCheckInfo HitCheckInfo)
{
	if (CharacterInfo->GetCharacterType() == ECharaterType::MainPlayer && Weapon && BasicMachine->GetCurrentStateID() == (int32)EMotionMachineState::Skill)
	{
		if (HitCheckInfo.ShapeType == EAtkShapeType::Box)
		{
			UClass* LoadOBJClass = UGameUtil::LoadClass<UClass>("/Game/Blueprints/BP_AtkBox.BP_AtkBox_C");
			if (!LoadOBJClass) return;
			AtkShape = g_pGameInstance->GetWorld()->SpawnActor<ACSAtkBox>(LoadOBJClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (AtkShape)
			{
				AtkShape->Init(this, Skill->GetCurSkill(), Skill->GetCurSkill()->GetCurComboClip(), HitCheckInfo);
				AtkShape->AtkCheck();
			}

		}
	}
}

void ACSCharacter::DestoryAtkShape()
{
	if (AtkShape)
	{
		UGameUtil::DestroyActor((AActor*)AtkShape);
	}
}



void ACSCharacter::ShowWeapon()
{
	if (Weapon)
	{
		if (bWeaponShow)return;
		bWeaponShow = true;
		Weapon->AttachToComponent(GetPawnMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), "HP_weapon_rSocket");
		//如果还在播显示动画，立即停止,并马上显示
		GetWorld()->GetTimerManager().ClearTimer(WeaponHideHandle);
		Weapon->Show(0);

	}
}

void ACSCharacter::HideWeapon(float Duration)
{
	if (Weapon)
	{
		if (!bWeaponShow)return;
		float HalfDuration = Duration / 2;
		bWeaponShow = false;
		//先隐藏
		Weapon->Hide(HalfDuration);
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([&]
			{
				if (this && Weapon && GetPawnMesh())
				{
					//再显示
					Weapon->AttachToComponent(GetPawnMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), "WeaponBackpack");
					Weapon->Show(0.5f);
				}
			});

		GetWorld()->GetTimerManager().SetTimer(WeaponHideHandle, TimerCallback, HalfDuration, false);
	}
}

void ACSCharacter::SetWeapon(ACSWeapon* InWeapon)
{
	Weapon = InWeapon;
	bWeaponShow = true;
	ShowWeapon();
}

ACSWeapon* ACSCharacter::GetWeapon()
{
	return Weapon;
}

EMotionMachineState ACSCharacter::GetCurMotionMachineState()
{
	return (EMotionMachineState)BasicMachine->GetCurrentStateID();
}

bool ACSCharacter::IsFighting()
{
	if (AtkTarget == nullptr)
	{
		return false;
	}
	if (AtkTarget->GetMotionMachine()->GetCurrentStateID() == (int32)EMotionMachineState::Dead)
	{
		return false;
	}
	return true;
}
