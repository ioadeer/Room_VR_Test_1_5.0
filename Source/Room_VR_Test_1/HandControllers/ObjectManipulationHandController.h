// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "ObjectManipulationHandController.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_VR_TEST_1_API AObjectManipulationHandController : public AHandControllerBase
{
	GENERATED_BODY()
	
public:
	AObjectManipulationHandController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void MainIndexTriggerAction() override;
	virtual void ThumbstickAction(FVector Value) override;


private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PositionMarker;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = true))
	class UStaticMesh* MeshedToBePlaced;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = true))
	float MovementScale = 10.f;

	//State
	FVector ObjectLocation;
	FVector PrevObjectLocation;

};
