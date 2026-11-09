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

	
}

void AAIPaddle::BeginPlay()
{
	Super::BeginPlay();
	Super::InitMovement();

	if (!Ball)
	{
		Ball = Cast<APongBall>(UGameplayStatics::GetActorOfClass(GetWorld(), APongBall::StaticClass()));

		if (!IsValid(Ball))
		{
			UE_LOG(LogAIPaddle, Error, TEXT("Ball reference is invalid, and no APongBall could be found during BeginPlay."))
		}
	}

	switch (Difficulty)
	{
	case EAIPaddleDifficulty::Easy:
		DifficultyData = EasyDifficultyData;
		break;

	case EAIPaddleDifficulty::Default:
		DifficultyData = DefaultDifficultyData;
		break;

	case EAIPaddleDifficulty::Hard:
		DifficultyData = HardDifficultyData;
		break;
	}

	MovementSpeedMultiplier = DifficultyData.PaddleSpeed;
}

void AAIPaddle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Ball) return;

	FVector Location = GetActorLocation();
	FVector BallLocation = Ball->GetActorLocation();
	FVector Forward = Ball->GetActorForwardVector();

	FVector NewLocation = Location;
	if (Location.Y > BallLocation.Y)
	{
		MoveUp(-1);
		//NewLocation.Y -= DifficultyData.PaddleSpeed;
	}
	else if (Location.Y < BallLocation.Y)
	{
		MoveUp(1);
		//NewLocation.Y += DifficultyData.PaddleSpeed;
	}
	//SetActorLocatin(NewLocation);

	FHitResult Hit;
	TArray<TWeakObjectPtr<const AActor>> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(Ball);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(IgnoredActors);

	bool bHit= GetWorld()->LineTraceSingleByChannel(Hit, BallLocation, BallLocation + (Forward * 500.0f), ECC_WorldStatic, Params);

	if (bHit)
	{	
#if WITH_EDITORONLY_DATA
		if (bDebugMode)
		{
			if (bDrawDebugLine)		DrawDebugLine(GetWorld(), BallLocation, Hit.Location, FColor::Blue, false, .1f);
			if (bDrawDebugSphere)	DrawDebugSphere(GetWorld(), Hit.Location, 10, 10, FColor::Red, false, .1f);
		}
#endif 
	}
	
}

void AAIPaddle::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Ball = nullptr;

	Super::EndMovement();
	Super::EndPlay(EndPlayReason);
}