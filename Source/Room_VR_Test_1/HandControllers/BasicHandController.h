// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "BasicHandController.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_VR_TEST_1_API ABasicHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:
	ABasicHandController();
	virtual void HandTriggered() override;
	virtual void HandPressed() override;
	virtual void MainIndexTriggerAction() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
