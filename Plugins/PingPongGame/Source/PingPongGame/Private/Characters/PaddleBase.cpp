// Copyright (c) 2026, Darshit Limbad

#include "Characters/PaddleBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogPaddleBase)

APaddleBase::APaddleBase()
{
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_None;
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
	AddActorWorldOffset(FVector(0.0f, 1.0f, 0.0f) * Velocity, true);

	UE_LOG(LogPaddleBase, Verbose, TEXT("Paddle current velocity: %f"), Velocity)	
}

void APaddleBase::EndMovement()
{
	bIsMoving = false;
	Velocity = 0.0f;
}