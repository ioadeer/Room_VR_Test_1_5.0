// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"

//#include "EnhancedInput/Public/InputMappingContext.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"


// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	//VRRoot->SetupAttachment(GetRootComponent());
	SetRootComponent(VRRoot);

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
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMappingContext.IsNull())
			{
				
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(),1);
				UE_LOG(LogTemp, Warning, TEXT("Setting input system context..."));
			}
		}
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	UE_LOG(LogTemp, Warning, TEXT("Binding actions..."));
	Input->BindAction(IA_RightTrigger, ETriggerEvent::Triggered, this, &AVRPawn::RightIndexTriggerAction);
	Input->BindAction(IA_LeftTrigger, ETriggerEvent::Triggered, this, &AVRPawn::LeftIndexTriggerAction);
	Input->BindAction(IA_RightThumbstick, ETriggerEvent::Triggered, this, &AVRPawn::RightThumbstickAction);
	Input->BindAction(IA_LeftThumbstick, ETriggerEvent::Triggered, this, &AVRPawn::LeftThumbstickAction);
	
}

void AVRPawn::RightIndexTriggerAction(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Warning, TEXT("Right index trigger action"));
	RightHandController->MainIndexTriggerAction();
}

void AVRPawn::LeftIndexTriggerAction(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Warning, TEXT("Left index trigger action"));
	LeftHandController->MainIndexTriggerAction();
}

void AVRPawn::RightThumbstickAction(const FInputActionValue& Value)
{
	// deberia tener mas validacion esto
	if (Value.GetMagnitude()!= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Right thumbstick %s"), *Value.ToString());
		FVector Temp(Value[0], Value[1], 0.f);
		RightHandController->ThumbstickAction(Temp);

	}

}

void AVRPawn::LeftThumbstickAction(const FInputActionValue& Value)
{
	// deberia tener mas validacion esto
	if (Value.GetMagnitude() != 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left thumbstick %s"), *Value.ToString());
		FVector Temp(Value[0], Value[1], 0.f);
		LeftHandController->ThumbstickAction(Temp);

	}
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

	UE_LOG(LogTemp, Warning, TEXT("Posicion Z %s"), *NewLocation.ToString());
	SetActorLocation(NewLocation);
	CameraManager->StartCameraFade(1, 0, FadeInTime, FColor(0));

	bCanTeleport = true;
}

UCameraComponent* AVRPawn::GetCamera()
{
	if (Camera)
	{
		return Camera;
	}
	return nullptr;
}
