// Fill out your copyright notice in the Description page of Project Settings.


#include "CSMainPlayer.h"
#include "Components/InputComponent.h"
#include "SkillSystem/CSSkillDefine.h"
#include "SkillSystem/CSSkillComponent.h"
#include "SkillSystem/CSSkill.h"
#include "BaseSystem/FSM/FSMState.h"
#include "BaseSystem/FSM/FSMMachine.h"
#include "BaseSystem/Ext/MathfExtLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseSystem/Ext/ActorInterpMovementComponent.h"
#include "SGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Other/CSPlayerSearchEnemyComponent.h"
#include "SkillSystem/CSComboClip.h"
#include "CSCharacter.h"
#include "Camera/CameraComponent.h"
#include "../FSM/CSRunState.h"


ACSMainPlayer::ACSMainPlayer()
{
}


void ACSMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACSMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACSMainPlayer::MoveRight);

	PlayerInputComponent->BindAction("NormalAtk", IE_Pressed, this, &ACSMainPlayer::NormalAttackPressed);
	PlayerInputComponent->BindAction("NormalAtk", IE_Released, this, &ACSMainPlayer::NormalAttackRelesed);
	PlayerInputComponent->BindAction("Skill1Atk", IE_Pressed, this, &ACSMainPlayer::Skill1Pressed);
	PlayerInputComponent->BindAction("Skill1Atk", IE_Released, this, &ACSMainPlayer::Skill1Relesed);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ACSMainPlayer::RollPressed);
	PlayerInputComponent->BindAction("Roll", IE_Released, this, &ACSMainPlayer::RollRelesed);
}

void ACSMainPlayer::MoveForward(float Val)
{
	RockerY = Val;
}

void ACSMainPlayer::MoveRight(float Val)
{
	RockerX = Val;
	if (RockerX != 0.f || RockerY != 0.f)
	{
		if (BasicMachine->GetCurrentStateID() != (int32)EMotionMachineState::Run)
		{
			ToRun(FVector::ZeroVector, 0);
		}
	}
	else
	{
		if (BasicMachine->GetCurrentStateID() == (int32)EMotionMachineState::Run)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600;
			ToIdle();
			MoveMessage(GetActorLocation(), CacheMaxSpeed);
		}
	}
}

void ACSMainPlayer::Move()
{
	FVector2D RockerInput = FVector2D(RockerX, RockerY);
	RockerSize = RockerInput.Size();
	if (RockerSize <= 0.5f)
	{
		RockerSize = 0.5f;
	}
	else
	{
		RockerSize = 1;
	}
	float Angle = UMathfExtLibrary::VectorAngle2D(RockerInput, FVector2D(0, 1));
	if (RockerX < 0)Angle = -Angle;
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw + Angle, 0);
	const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
	AddMovementInput(Direction, RockerSize);

	AddControllerYawInput(RockerX * 0.15f);

	if (CacheMaxSpeed == 0)
	{
		CacheMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	//发送移动消息
	FVector TargetPosition = CalMoveMessageTarget();
	float NewMaxSpeed = 0;
	if (RockerSize <= 0.1f)
	{
		NewMaxSpeed = CacheMaxSpeed * 0.5f;
	}
	else if (RockerSize <= 1)
	{
		NewMaxSpeed = CacheMaxSpeed;
	}
	MoveMessage(TargetPosition, NewMaxSpeed);
}

void ACSMainPlayer::NormalAttackPressed()
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::NormalAttackBtn, EInputStatus::Press, 10000);
	}
}

void ACSMainPlayer::NormalAttackRelesed()
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::NormalAttackBtn, EInputStatus::Relese, 10000);
	}
}


void ACSMainPlayer::Skill1Pressed()
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::SkillAttackBtn, EInputStatus::Press, 101);
	}
}

void ACSMainPlayer::Skill1Relesed()
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::SkillAttackBtn, EInputStatus::Relese, 101);
	}
}

void ACSMainPlayer::SkillPressed(int32 SkillId)
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::SkillAttackBtn, EInputStatus::Press, SkillId);
	}
}

void ACSMainPlayer::SkillRelesed(int32 SkillId)
{
	if (Skill)
	{
		Skill->RespondInput(ESkillButtonType::SkillAttackBtn, EInputStatus::Relese, SkillId);
	}
}

void ACSMainPlayer::RollPressed()
{
	UCSRunState* RunState = BasicMachine->GetState<UCSRunState>((int32)EMotionMachineState::Run);
	if (RunState)
	{
		RunState->SetIsSprint(true);
	}
	CacheMaxSpeed = 0;
	GetCharacterMovement()->MaxWalkSpeed = 1200;
	ToRoll();
}

void ACSMainPlayer::RollRelesed()
{
	UCSRunState* RunState = BasicMachine->GetState<UCSRunState>((int32)EMotionMachineState::Run);
	if (RunState)
	{
		RunState->SetIsSprint(false);
	}
	CacheMaxSpeed = 0;
	GetCharacterMovement()->MaxWalkSpeed = 600;
}


void ACSMainPlayer::PrepareComboClip()
{
	Super::PrepareComboClip();
	//查找敌人，缓存朝向数据
	FComboClipsTableData* Data = Skill->GetCurSkill()->GetCurComboClip()->GetClipData();
	FComboJumpLineData PreLineData = Skill->GetCurSkill()->GetCurComboClip()->GetPreLineData();
	if (SearchEnemy)
	{
		UCSPlayerSearchEnemyComponent* PlayerSearhEnemy = Cast<UCSPlayerSearchEnemyComponent>(SearchEnemy);
		if (PlayerSearhEnemy)
		{
			int32 ComboID = Data->ID;
			PlayerSearhEnemy->SetUpParameters(PreLineData.LockRange, PreLineData.FirsetLock, PreLineData.NoRockerAngleLimit, PreLineData.HasRockerAngleLimit, PreLineData.MaxRotateAngle, FVector2D(RockerX, RockerY));
			CacheMaxRotate = PreLineData.MaxRotateAngle;
			AActor* Enemy = PlayerSearhEnemy->ExecuteSearchEnemy();
			if (Enemy && PlayerSearhEnemy->GetTargetDirection() != FRotator::ZeroRotator)
			{
				float EndYaw = PlayerSearhEnemy->GetTargetDirection().Yaw;
				CacheTargetYaw = EndYaw;
			}
			else
			{
				CacheTargetYaw = -9999;
			}
		}
	}

	//缓存操作数据
	CacheRockerX = RockerX;
	CacheRockerY = RockerY;
	CacheSourceYaw = GetActorRotation().Yaw;
	CacheControllerYaw = GetControlRotation().Yaw;
}



void ACSMainPlayer::MoveMessage(FVector Target, float MaxWalkSpeed)
{
	moveV2::ReqMove WalkMesaage;
	WalkMesaage.set_x(GetActorLocation().X);
	WalkMesaage.set_y(GetActorLocation().Y);
	WalkMesaage.set_z(GetActorLocation().Z);
	WalkMesaage.set_ox(Target.X);
	WalkMesaage.set_oy(Target.Y);
	WalkMesaage.set_oz(Target.Z);
	WalkMesaage.set_speed(MaxWalkSpeed);
	WalkMesaage.set_action(0);
	g_pGameInstance->SendMessage(68001, &WalkMesaage);

}

void ACSMainPlayer::SendMoveMessage(float DeltaTime)
{



}

void ACSMainPlayer::SendComboMessage(int32 ComboId)
{
	fightV2::ComboNode ComboNode;
	ComboNode.set_combonode(ComboId);
	ComboNode.set_dir(GetActorRotation().Yaw);
	ComboNode.set_x(GetActorLocation().X);
	ComboNode.set_y(GetActorLocation().Y);
	ComboNode.set_z(GetActorLocation().Z);
	ComboNode.set_rockerx(CacheRockerX);
	ComboNode.set_rockery(CacheRockerY);
	ComboNode.set_controlleryaw(CacheControllerYaw);
	ComboNode.set_targetdir(CacheTargetYaw);
	ComboNode.set_maxrotate(CacheMaxRotate);
	g_pGameInstance->SendMessage(69014, &ComboNode);
}


void ACSMainPlayer::SendComboEndMessage()
{
	fightV2::ComboEnd ComboEnd;
	g_pGameInstance->SendMessage(69016, &ComboEnd);
}

FVector ACSMainPlayer::GetMainPlayrCameraPos()
{
	UClass* CamerClass = UCameraComponent::StaticClass();
	UCameraComponent* Camera = Cast<UCameraComponent>(GetComponentByClass(CamerClass));
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}
	return FVector::ZeroVector;
}


