// Copyright (c) 2026, Darshit Limbad

#include "Characters/PaddleBase.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogPaddleBase)

APaddleBase::APaddleBase()
{
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
}

bool APaddleBase::TryGetVelocity(float& OutVelocity) const
{
	if (GetIsMoving())
	{
		OutVelocity = GetVelocityValue();
		return true;
	}
	return false;
}

void APaddleBase::InitMovement()
{
	bIsMoving = true;
}

void APaddleBase::MoveUp(float MoveInput)
{
	if (!bIsMoving) return;

	Velocity = MoveInput * GetMovementSpeedMultiplier();
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Velocity);

	UE_LOG(LogPaddleBase, Verbose, TEXT("Paddle current velocity: %f"), Velocity)	
}

void APaddleBase::EndMovement()
{
	bIsMoving = false;
	Velocity = 0.0f;
}