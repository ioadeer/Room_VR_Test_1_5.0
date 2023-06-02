// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManipulationHandController.h"
#include "Engine/StaticMeshActor.h"

AObjectManipulationHandController::AObjectManipulationHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	PositionMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position Marker"));
	PositionMarker->SetupAttachment(GetRootComponent());
}

void AObjectManipulationHandController::BeginPlay()
{
	Super::BeginPlay();
	PrevObjectLocation = PositionMarker->GetRelativeLocation();
	
}

void AObjectManipulationHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Object location before assigning to marker %s"), *ObjectLocation.ToString());
	PositionMarker->SetWorldLocation(ObjectLocation);
	if (ObjectLocation != PrevObjectLocation)
	{
	}
	PrevObjectLocation = ObjectLocation;
	//UE_LOG(LogTemp, Warning, TEXT("Object location before assigning to marker %s"), PositionMarker->getactor.ToString());
}

void AObjectManipulationHandController::MainIndexTriggerAction()
{
	if (MeshedToBePlaced)
	{
		AStaticMeshActor* MyNewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		MyNewActor->SetMobility(EComponentMobility::Movable);
		MyNewActor->SetActorLocation(ObjectLocation);
		
		UStaticMeshComponent* MeshComponent = MyNewActor->GetStaticMeshComponent();
		if (MeshComponent)
		{
			MeshComponent->SetStaticMesh(MeshedToBePlaced);
		}
	}
}

void AObjectManipulationHandController::ThumbstickAction(FVector Value)
{
	ObjectLocation.X += Value.Y * MovementScale;
	ObjectLocation.Y += Value.X * MovementScale;
	ObjectLocation.Z = 1.f;
	UE_LOG(LogTemp, Warning, TEXT("Object location value on class callback %s"), *ObjectLocation.ToString());
}
