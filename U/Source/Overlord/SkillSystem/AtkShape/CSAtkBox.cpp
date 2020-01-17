// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAtkBox.h"

// Sets default values
ACSAtkBox::ACSAtkBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

void ACSAtkBox::Init(ACSCharacter* InOwner, UCSSkill* InSkill, UCSComboClip* InClip, FHitCheckInfo InHitChekInfo)
{
	Super::Init(InOwner, InSkill, InClip, InHitChekInfo);
	LocaPosOffset = InHitChekInfo.LocaPosOffset;
	ScaleX = InHitChekInfo.ScaleX;
	ScaleY = InHitChekInfo.ScaleY;
	ScaleZ = InHitChekInfo.ScaleZ;
}


