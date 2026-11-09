// Copyright (c) 2026, Darshit Limbad

#include "Characters/PlayerPaddle.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerPaddle)

void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerPaddle::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PC = Cast<APlayerController>(NewController);
	if (PC && IMC)
	{

		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(IMC, 0);
		}
	}
}

void APlayerPaddle::UnPossessed()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			InputSubsystem->RemoveMappingContext(IMC);
		}
	}

	Super::UnPossessed();
}

void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementIA, ETriggerEvent::Started, this, &ThisClass::InitMovement);
		EnhancedInputComponent->BindAction(MovementIA, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveUp);
		EnhancedInputComponent->BindAction(MovementIA, ETriggerEvent::Completed, this, &ThisClass::EndMovement);
	}
}

void APlayerPaddle::EndPlay(EEndPlayReason::Type EndPlay)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{

		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			InputSubsystem->RemoveMappingContext(IMC);
		}
	}

	Super::EndPlay(EndPlay);
}

void APlayerPaddle::HandleMoveUp(const FInputActionValue& Value)
{
	float MoveInput = Value.Get<float>();
	MoveUp(MoveInput);
}