// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkillSystem/CSSkillDefine.h"
#include "Protoc/test.pb.h"
#include "BaseDefine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Protoc/fight.pb.h"
#include "Animation/AnimInstance.h"
#include "SkillSystem/SkillReciver/CSSkillReceiverInterface.h"
#include "SkillSystem/CSSkillData.h"
#include "Engine/EngineTypes.h"
#include "CSCharacter.generated.h"


#define  MESSAGEFLOATMULTIPLE 1.f

class USkeletalMeshComponent;
class UCSSkillComponent;
class USpringArmComponent;
class UCSSearchEnemyComponent;
class UFSMMachine;
class UActorInterpMovementComponent;
class UCSHead;
class UWidgetComponent;
class UCSGameCharacter;
class ACSWeapon;
class UCSSkillResult;
class ACSAtkShapeBase;
class UCameraComponent;
class UMaterialInstanceDynamic;
/*
Basic Machine Transition
*/
UENUM(BlueprintType)
enum class EMotionMachineTransition :uint8
{
	None = 0,
	ToIdle,
	ToRun,
	ToSkill,
	ToRoll,
	ToHurt,
	ToDead
};


/*
*Basic Machine State
*/
UENUM(BlueprintType)
enum class EMotionMachineState :uint8
{
	None = 0,
	Idle,
	Run,
	Skill,
	Roll,
	Hurt,
	Dead
};

/*
*Space Machine State
*/
UENUM(BlueprintType)
enum class ESpaceMachineState :uint8
{
	None = 0,
	Ground,
	Air
};


/*
*Space Machine State
*/
UENUM(BlueprintType)
enum class ESpaceMachineTransition :uint8
{
	None = 0,
	ToGround,
	ToAir
};


UCLASS()
class OVERLORD_API ACSCharacter : public ACharacter, public ICSSkillReceiverInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSCharacter();

	//////////////////////////////////////////////////////////////////////////
	// Life Function
protected:
	virtual void BeginPlay() override;
public:
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void Init(UCSGameCharacter* InCharacterInfo);
	//////////////////////////////////////////////////////////////////////////
	// 自定义运动
public:
	void InterpMoveToTarget(FVector TargetRot, float Time);
	void StopMove();

	//////////////////////////////////////////////////////////////////////////
	// 动画
public:
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate /* = 1.f */, FName StartSectionName /* = NAME_None */) override;
	virtual void  PlayMontage(FString MontageName, UAnimMontage*& OutAnimMontag, FOnMontageEnded EndDelegate);
	virtual void  StopAnimMontage(class UAnimMontage* AnimMontage) override;
	virtual void  StopAllAnimMontages(float BlendTime = 0.25f);
	int32 GetAnimationMode();
	virtual void StopAniFrame(float Dealy, float PauseTime);
	virtual void ResetAniFrame();
protected:
	virtual void SetAniRateScale(float AniRate);
protected:
	FTimerHandle DelayStopAniHandle;
	FTimerHandle DoStopAniHandle;
	UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial;
public:
	FORCEINLINE UMaterialInstanceDynamic* GetMaterialInstanceDynamic() { return DynamicMaterial; }
	//////////////////////////////////////////////////////////////////////////
	// 外观设置
public:
	USkeletalMeshComponent* GetPawnMesh() const;									//获取Mesh组件
	UFUNCTION(BlueprintCallable, Category = Mesh)
		virtual bool IsMainPlayer() const;											//是否是本地玩家
	bool IsAlive() const;															//是否存活
	void SetSkeletalMesh(USkeletalMesh* NewSkeletalMesh,							//设置模型
		FVector Scale3D,
		EAnimationMode::Type AnimMode,
		FString AnimInstanceName,
		TArray<FMaterialData> Materials,
		bool bReinitPose = true);

public:
	void StartInterpMeshLocation(FVector From, FVector To, float InterpTime);
	void StopInterpMeshLocation();
protected:
	void UpdateInterpMeshLocation(float DelatTime);
	FVector InterpMeshFrom;
	FVector InterpMeshTo;
	float InterpMeshTimer;
	float InterpMeshTime;
	bool bInterpMeshLocation;
	float LocalMeshZ;
	//////////////////////////////////////////////////////////////////////////
	// 摄像机设置
public:
	UFUNCTION(BlueprintCallable, Category = Camera) void AdjustCameraBoomArmLength(float MinLength, float MaxLength, float Speed);
	virtual void MoveToTargetPosition(FVector TargetPostion, float MaxSpeed);	   //移动到目标点	

	//////////////////////////////////////////////////////////////////////////
	// 状态机
protected:
	virtual void ConstructionMotionMachine();										//构建状态机

protected:
	UPROPERTY()	UFSMMachine* BasicMachine;											//基础状态机
	UPROPERTY()	UFSMMachine* SpaceMachine;											//空间状态
public:
	virtual void PrepareComboClip();
	virtual void ToIdle();
	virtual void ToRun(FVector RunTarget, float MoveSpeed);
	virtual void ToHurt(FString HurtPath);
	virtual void ToDead(ACSCharacter* Attacker);
	virtual bool ToSkill(FString ComboPath);
	virtual void ToRoll();
	virtual void ForcibleToIdle();													//强制回到Idle状态
	virtual bool CanReleseSkill();													//是否可以释放技能

protected:
	bool bCanRoll = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Motion) float MaxRollSpeed = 600;

public:
	FORCEINLINE virtual float GetMaxRollSpeed() { return MaxRollSpeed; }
	FORCEINLINE virtual UFSMMachine* GetMotionMachine() { return BasicMachine; }
	FORCEINLINE virtual void SetCanRoll(bool InCanRoll) { bCanRoll = InCanRoll; }

	UFUNCTION(BlueprintPure) bool GetIsFightIdle();
	virtual bool GetIsSprint();														//是否是冲刺
	//////////////////////////////////////////////////////////////////////////
	// 技能相关接口
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
		void ReceiverSkill(FSkillReceiveInfo ReceiveInfo);
	virtual void ReceiverSkill_Implementation(FSkillReceiveInfo ReceiveInfo) override;

	virtual void ReleaseSkillData();
	//////////////////////////////////////////////////////////////////////////
	// 武器相关
public:
	TWeakObjectPtr<ACSCharacter> SkillController;						//控制者
	void CreateAtkShape(FHitCheckInfo HitCheckInfo);
	void DestoryAtkShape();
	void ShowWeapon();
	void HideWeapon(float Duration);
	UPROPERTY(BlueprintReadWrite, Category = Weapon) ACSWeapon* Weapon;
	UPROPERTY(BlueprintReadWrite, Category = Weapon) ACSAtkShapeBase* AtkShape;
	void SetWeapon(ACSWeapon* InWeapon);
	UFUNCTION(BlueprintPure) ACSWeapon* GetWeapon();
protected:
	bool bWeaponShow;
	FTimerHandle WeaponHideHandle;
	//////////////////////////////////////////////////////////////////////////
	// 效果
	//UFUNCTION() void Repulse(FVector Target);
	//UFUNCTION() void Diaup(FVector Target);
	//////////////////////////////////////////////////////////////////////////
	// Component
protected:
	/** 碰撞体 */
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)	UPrimitiveComponent* Primitive;
	/** 技能引擎 */
	UPROPERTY(VisibleDefaultsOnly, Category = Skill)	UCSSkillComponent* Skill;
	/** 摄像机 */
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)	USpringArmComponent* CameraBoom;
	/** 摄像机 */
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)	UCameraComponent* CameraComponent;
	/** 自动索敌机制 */
	UPROPERTY(VisibleDefaultsOnly, Category = Search)	UCSSearchEnemyComponent* SearchEnemy;
	/** 自定义角色线性插值移动 */
	UPROPERTY(VisibleDefaultsOnly, Category = Move)		UActorInterpMovementComponent* InterpMovement;
	/* 角色UI */
	UPROPERTY(VisibleDefaultsOnly, Category = UI)		UWidgetComponent* HeadWidget;
	/* 头顶组件管理 */
	UPROPERTY(VisibleDefaultsOnly, Category = UI)		UCSHead* Head;
	/* 角色信息 */
	UPROPERTY(VisibleDefaultsOnly, Category = Info)		UCSGameCharacter* CharacterInfo;
	/* 技能结算逻辑 */
	UPROPERTY() UCSSkillResult* SkillResult;

public:
	UFUNCTION(BlueprintPure, Category = Skill) FORCEINLINE UCSSkillComponent* GetSkillComponent() { return Skill; }
	UFUNCTION(BlueprintPure, Category = Move) FORCEINLINE UActorInterpMovementComponent* GetInterpMovement() { return InterpMovement; }
	UFUNCTION(BlueprintPure, Category = Head) FORCEINLINE UWidgetComponent* GetHeadWidget() { return HeadWidget; }
	UFUNCTION(BlueprintPure, Category = Head) FORCEINLINE UCSHead* GetHead() { return Head; }
	UFUNCTION(BlueprintPure, Category = Info) FORCEINLINE UCSGameCharacter* GetCharacterInfo() { return CharacterInfo; }
	UFUNCTION(BlueprintPure, Category = Skill) FORCEINLINE UCSSkillResult* GetSkillResult() { return SkillResult; }
	UFUNCTION(BlueprintPure, Category = Camera) FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	UFUNCTION(BlueprintPure, Category = Camera) FORCEINLINE UCameraComponent* GetCamera() { return CameraComponent; }
public:
	EMotionMachineState GetCurMotionMachineState();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test) float TestComboRotateTime = 0.1f;
	//////////////////////////////////////////////////////////////////////////
	// 其他(测试)
	UFUNCTION(BlueprintImplementableEvent, Category = Move) FVector CalNextTarget(FVector OriginPos);
	UFUNCTION(BlueprintImplementableEvent, Category = Move) void SpawnDamgeHP(float HP);
	
	UPROPERTY() ACSCharacter* AtkTarget;

	UFUNCTION(BlueprintCallable, Category = Move) bool IsFighting();
};