// Copyright (c) 2026, Darshit Limbad

#include "Characters/AIPaddle.h"
#include "Actors/PongBall.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogAIPaddle)

AAIPaddle::AAIPaddle()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	switch (Difficulty)
	{
	case EAIPaddleDifficulty::Easy:
		DifficultyData = { ReactionTime, DeadZone, PaddleSpeed};
		break;

	case EAIPaddleDifficulty::Medium:
		DifficultyData = { 0.10f, 20.f, 300.f };
		break;

	case EAIPaddleDifficulty::Hard:
		DifficultyData = { 0.05f, 5.f, 600.f };
		break;
	}

	SetActorTickInterval(DifficultyData.ReactionTime);
	GetCharacterMovement()->MaxWalkSpeed = DifficultyData.PaddleSpeed;
}

void AAIPaddle::BeginPlay()
{
	Super::BeginPlay();

	if (!Ball)
	{
		Ball = Cast<APongBall>(UGameplayStatics::GetActorOfClass(GetWorld(), APongBall::StaticClass()));

		if (!IsValid(Ball))
		{
			UE_LOG(LogAIPaddle, Warning, TEXT("Ball reference is invalid, and no APongBall could be found during BeginPlay."))
		}
	}

	Super::NotifyMovementStarted();
}

void AAIPaddle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Ball) return;

	const float BallY = Ball->GetYCoordinate();
	const float PaddleY = GetActorLocation().Y;
	
	if (FMath::Abs(BallY - PaddleY) >= DifficultyData.DeadZone)
	{
		MoveUp((BallY > PaddleY) ? 1.0f : -1.0f);
	}
}

void AAIPaddle::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::NotifyMovementEnded();
	Ball = nullptr;

	Super::EndPlay(EndPlayReason);
}