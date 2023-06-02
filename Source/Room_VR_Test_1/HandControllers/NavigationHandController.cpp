// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationHandController.h"
#include "../VRPawn.h"
#include "Camera/CameraComponent.h"

ANavigationHandController::ANavigationHandController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ANavigationHandController::BeginPlay()
{
    Super::BeginPlay();
}

void ANavigationHandController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANavigationHandController::MainIndexTriggerAction()
{
    UE_LOG(LogTemp, Warning, TEXT("Main Index Trigger Action not yet implemented"));
}

void ANavigationHandController::ThumbstickAction(FVector Value)
{
    auto ControllerOwner = Cast<AVRPawn>(GetOwner());
    if (ControllerOwner)
    {
        FRotator CurrentRotation = ControllerOwner->GetActorRotation();
        CurrentRotation.Yaw += Value.X * RotationScale;
        ControllerOwner->SetActorRotation(CurrentRotation);

        FVector CurrentLocation = ControllerOwner->GetActorLocation();
        UCameraComponent* TempCamera = ControllerOwner->GetCamera();
        if (TempCamera)
        {
            FVector CameraFV = TempCamera->GetForwardVector();
            CurrentLocation.X += CameraFV.X * Value.Y * MovementScale;
            CurrentLocation.Y += CameraFV.Y * Value.Y * MovementScale;
            ControllerOwner->SetActorLocation(CurrentLocation);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Temp Camera Casted from Pawn"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No owner of class VRPawn found for Navigation Hand Controller"));
    }
}
