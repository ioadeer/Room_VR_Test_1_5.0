// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HandControllers/HandControllerBase.h"
#include "VRPawn.generated.h"

UCLASS()
class ROOM_VR_TEST_1_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartTeleport(FVector Destination) { if (bCanTeleport) BeginTeleport(Destination); };

private:
	//Declaration
	UPROPERTY()
	class UCameraComponent* Camera;

	UPROPERTY()
	class USceneComponent* VRRoot;

	UPROPERTY()
	AHandControllerBase* RightHandController;

	UPROPERTY()
	AHandControllerBase* LeftHandController;

	class APlayerCameraManager* CameraManager;


	//Configuration
	// Aca se pasa blueprint de Hand Controller
	UPROPERTY(EditDefaultsOnly, DisplayName="Right Controller BP class")
	TSubclassOf<AHandControllerBase> RightHandControllerClass;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Left Controller BP class")
	TSubclassOf<AHandControllerBase> LeftHandControllerClass;

	UPROPERTY(EditAnywhere)
	float PlayerStartZOffset = 100.f;

	UPROPERTY(EditAnywhere)
	float FadeOutTime = 0.25f;

	UPROPERTY(EditAnywhere)
	float FadeInTime = 0.25f;

	//Methods
	void RightIndexTriggerAction();
	void LeftIndexTriggerAction();

	void BeginTeleport(FVector Destination);
	void Teleport(FVector Destination);

	// State
	bool bCanTeleport = true;
	
};
