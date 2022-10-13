// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportHandController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "NavigationSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "../VRPawn.h"

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
    Super::BeginPlay();
    DestinationMarker->SetVisibility(false);
}

void ATeleportHandController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateDestinationMarker();
}

void ATeleportHandController::MainIndexTriggerAction()
{
    auto ControllerOwner = Cast<AVRPawn>(GetOwner());
    if (ControllerOwner)
    {
        ControllerOwner->StartTeleport(Destination);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No owner of class VRPawn found for Teleport Hand Controller"));
    }
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
                for (FPredictProjectilePathPointData const& PointData : Result.PathData)
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

void ATeleportHandController::UpdateSpline(const TArray<FVector>& Path)
{
    TeleportPath->ClearSplinePoints(false);
    for (int32 i = 0; i < Path.Num(); ++i) {
        FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
        FSplinePoint Point(i, LocalPosition, ESplinePointType::Curve);
        TeleportPath->AddPoint(Point, false);
    }
    TeleportPath->UpdateSpline();
}

void ATeleportHandController::DrawTeleportPath(const TArray<FVector>& Path)
{
    UpdateSpline(Path);

    for (USplineMeshComponent* SplineMesh : TeleportPathMeshPool)
    {
        SplineMesh->SetVisibility(false);
    }
    int32 SegmentNum = Path.Num() - 1;
    for (int32 i = 0; i < SegmentNum; i++)
    {
        if (TeleportPathMeshPool.Num() <= i)
        {
            USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
            SplineMesh->SetMobility(EComponentMobility::Movable);
            SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
            SplineMesh->SetStaticMesh(TeleportArchMesh);
            SplineMesh->SetMaterial(0, TeleportArchMaterial);
            SplineMesh->RegisterComponent();

            TeleportPathMeshPool.Add(SplineMesh);
        }
        if (TeleportPathMeshPool.IsValidIndex(i))
        {
            USplineMeshComponent* SplineMesh = TeleportPathMeshPool[i];
            SplineMesh->SetVisibility(true);
            FVector StartPos, StartTangent, EndPos, EndTangent;
            TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
            TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);
            SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Teleport Path Mesh Pool index out of bounds."));
        }
    }
}

void ATeleportHandController::UpdateDestinationMarker()
{
    TArray<FVector> Path;
    FVector Location;
    if (FindTeleportDestination(Path, Location))
    {
        DestinationMarker->SetVisibility(true);
        DestinationMarker->SetWorldLocation(Location);
        Destination = Location;
        DrawTeleportPath(Path);
    }
    else
    {
        DestinationMarker->SetVisibility(false);
        TArray<FVector> EmptyPath;
        DrawTeleportPath(EmptyPath);
    }
}