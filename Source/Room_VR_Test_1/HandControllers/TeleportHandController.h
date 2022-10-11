// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "TeleportHandController.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_VR_TEST_1_API ATeleportHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:
    ATeleportHandController();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
       

private: 
    //methods
    // 
    bool FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation);
    void UpdateDestinationMarker();
    void UpdateSpline(const TArray<FVector>& Path);
    void DrawTeleportPath(const TArray<FVector>& Path);
   
    // Components
    UPROPERTY()
    class USceneComponent* VRRoot;

    UPROPERTY(VisibleAnywhere)
    class USplineComponent* TeleportPath;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* DestinationMarker;

    UPROPERTY()
    TArray<class USplineMeshComponent*> TeleportPathMeshPool;
    
 // config parameter

    UPROPERTY(EditAnywhere)
    float TeleportProjectileRadius = 10.f;

    UPROPERTY(EditAnywhere)
    float TeleportProjectileSpeed = 800.f;

    UPROPERTY(EditAnywhere)
    float TeleportSimulationTime = 2.f;

    UPROPERTY(EditAnywhere)
    FVector TeleportProjectionExtent = FVector(100.f, 100.f, 100.f);

    UPROPERTY(EditDefaultsOnly)
    class UStaticMesh* TeleportArchMesh;

    UPROPERTY(EditDefaultsOnly)
    class UMaterialInterface* TeleportArchMaterial;
        
};
