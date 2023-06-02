// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "NavigationHandController.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_VR_TEST_1_API ANavigationHandController : public AHandControllerBase
{
	GENERATED_BODY()

	ANavigationHandController();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual void MainIndexTriggerAction() override;
    virtual void ThumbstickAction(FVector Value) override;

private:
    UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = true))
    float RotationScale = 2.f;

    UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = true))
    float MovementScale = 10.f;
};
