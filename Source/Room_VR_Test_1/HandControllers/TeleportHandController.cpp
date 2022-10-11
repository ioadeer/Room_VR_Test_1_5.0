// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportHandController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "NavigationSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

ATeleportHandController::ATeleportHandController()
{
    PrimaryActorTick.bCanEverTick = true;
	    
    TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
    TeleportPath->SetupAttachment(GetRootComponent());

    DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
    DestinationMarker->SetupAttachment(GetRootComponent());

}

void ATeleportHandController::BeginPlay()
{
}

void ATeleportHandController::Tick(float DeltaTime)
{
}

bool ATeleportHandController::FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation)
{
    FVector Start = GetActorLocation();
    FVector Look = GetActorForwardVector();
    Look = Look.RotateAngleAxis(30, GetActorRightVector());

    FPredictProjectilePathParams Params(
        TeleportProjectileRadius,
        Start,
        Look * TeleportProjectileSpeed,
        TeleportSimulationTime,
        ECollisionChannel::ECC_Visibility,
        this
    );
    // Draw debug line for Projectile Path
    // Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
    Params.bTraceComplex = true;
    FPredictProjectilePathResult Result;
    if (UGameplayStatics::PredictProjectilePath(
        this,
        Params,
        Result
    )
        ) {
        FNavLocation NavLocation;
        UWorld* MyWorld = GetWorld();
        if (MyWorld)
        {
            if (UNavigationSystemV1::GetCurrent(MyWorld)->ProjectPointToNavigation(
                Result.HitResult.Location,
                NavLocation,
                TeleportProjectionExtent
            ))
            {
                for (FPredictProjectilePathPointData PointData : Result.PathData)
                {
                    OutPath.Add(PointData.Location);
                }
                OutLocation = NavLocation.Location;
                return true;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NULL POINTER TO WORLD"));
            return false;
        }
        return false;
    }
    else {
        return false;
    }
}
