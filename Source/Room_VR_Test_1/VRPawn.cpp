// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0));
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Subtract default Player Start Z position
	FVector TempActorLocation = GetActorLocation();
	TempActorLocation.Z -= PlayerStartZOffset;
	SetActorLocation(TempActorLocation);

	//Setup Hand Controllers
	RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(RightHandControllerClass);
	if (RightHandController != nullptr)
	{
		RightHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightHandController->SetHand(EControllerHand::Right);
		RightHandController->SetOwner(this);
	}

	LeftHandController = GetWorld()->SpawnActor<AHandControllerBase>(LeftHandControllerClass);
	if (LeftHandController != nullptr)
	{
		LeftHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftHandController->SetHand(EControllerHand::Left);
		LeftHandController->SetOwner(this);
	}
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z -= 0.f;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("RightIndexTrigger"), EInputEvent::IE_Released, this, &AVRPawn::RightIndexTriggerAction);
	PlayerInputComponent->BindAction(TEXT("LeftIndexTrigger"), EInputEvent::IE_Released, this, &AVRPawn::LeftIndexTriggerAction);

}

void AVRPawn::RightIndexTriggerAction() {
	RightHandController->MainIndexTriggerAction();
}

void AVRPawn::LeftIndexTriggerAction() {
	LeftHandController->MainIndexTriggerAction();
}

void AVRPawn::BeginTeleport(FVector Destination)
{
	bCanTeleport = false;

	if (CameraManager)
	{
		CameraManager->StartCameraFade(0, 1, FadeOutTime, FColor(0));
		FTimerHandle PlayerTeleportHandle;
		FTimerDelegate PlayerTeleportDelegate = FTimerDelegate::CreateUObject(
			this,
			&AVRPawn::Teleport,
			Destination
		);
		UWorld* MyWorld = GetWorld();
		if (!MyWorld)
		{
			UE_LOG(LogTemp, Warning, TEXT("NULL POINTER TO WORLD"));
			return;
		}
		GetWorld()->GetTimerManager().SetTimer(PlayerTeleportHandle, PlayerTeleportDelegate, FadeOutTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Null Pointer to Camera Manager"));
	}

}

void AVRPawn::Teleport(FVector Destination)
{
	FVector NewLocation = Destination;
	// NewLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//NewLocation += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorUpVector();
	SetActorLocation(NewLocation);
	CameraManager->StartCameraFade(1, 0, FadeInTime, FColor(0));

	bCanTeleport = true;
}