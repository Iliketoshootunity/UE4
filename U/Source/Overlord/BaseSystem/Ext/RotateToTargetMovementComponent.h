// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "RotateToTargetMovementComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class OVERLORD_API URotateToTargetMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:
	URotateToTargetMovementComponent();
protected:
	/** How fast to update roll/pitch/yaw of the component we update. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateToTargetComponent)
		float RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateToTargetComponent)
		FRotator EndRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateToTargetComponent)
		FVector PivotTranslation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateToTargetComponent)
		uint32 bRotationInLocalSpace : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateToTargetComponent)
		uint32 bStartOnPlay : 1;

private:
	//Ðý×ªÖá
	FVector RotatePivot;
	FRotator Temp;
	bool bRoatating = false;
public:
	UFUNCTION(BlueprintCallable)
		void StartRotate(FRotator EndRotate, float RotateRotate);
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
