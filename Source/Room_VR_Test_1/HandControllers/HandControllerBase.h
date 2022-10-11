// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "HandControllerBase.generated.h"

UCLASS()
class ROOM_VR_TEST_1_API AHandControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandControllerBase();
	void SetHand(EControllerHand Hand) { MotionController->SetTrackingSource(Hand); }
	virtual void HandTriggered() {};
	virtual void HandPressed() {};

protected:
	
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;
};
