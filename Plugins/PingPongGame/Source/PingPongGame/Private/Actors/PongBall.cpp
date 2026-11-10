// Copyright (c) 2026, Darshit Limbad

#include "Actors/PongBall.h"
#include "Characters/PaddleBase.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

DEFINE_LOG_CATEGORY(LogPongBall)

APongBall::APongBall()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->InitSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));

	RootComponent = SphereComponent;

	RenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Render Component"));
	RenderComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	RenderComponent->Mobility = EComponentMobility::Movable;
	RenderComponent->SetSimulatePhysics(false);
		
	RenderComponent->SetupAttachment(SphereComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.DisplayName = FText::FromString("Arrow");
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA

}

void APongBall::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateDirection();
	
	Velocity = StartSpeed;
}

void APongBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveBall(DeltaSeconds);
}

void APongBall::EndPlay(EEndPlayReason::Type EndPlayReason)
{

	SphereComponent->OnComponentHit.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
void APongBall::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Max speed should always be greater or equal to start speed
	MaxSpeed = FMath::Max(MaxSpeed, StartSpeed);
}
#endif

void APongBall::MoveBall(float DeltaSeconds)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * Velocity * DeltaSeconds);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(Hit, StartLocation, EndLocation, FQuat::Identity,
		ECC_GameTraceChannel1 /** Ball */, FCollisionShape::MakeSphere(SphereComponent->GetScaledSphereRadius()), Params);

	if (bHit)
	{
#if !UE_BUILD_SHIPPING
		if (bDebugMode)
		{
			DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 1.0f);
		}
#endif

		FVector SafeLocation = Hit.Location + (Hit.ImpactNormal * 2.0f);
		SetActorLocation(SafeLocation, false);

		HandleBallHit(Hit);
	}
	else
	{
		SetActorLocation(EndLocation, false);
	}

}

void APongBall::UpdateDirection()
{
	FVector Forward = GetActorForwardVector();
	Direction = FVector(Forward.X, Forward.Y, 0.0f);
	SetHorizontalDirection(Direction.X > 0 ? EBallDirection::Left : EBallDirection::Right);

	UE_LOG(LogPongBall, Verbose, TEXT("UpdateDirection: New Direction: X: %f Y: %f"), Direction.X, Direction.Y);
}

void APongBall::SetHorizontalDirection(EBallDirection NewDirection)
{
	HorizontalDirection = NewDirection;
}

void APongBall::HandleBallHit(const FHitResult& Hit)
{
	if (!Hit.GetActor()) return;

	if (Hit.GetActor()->ActorHasTag(TEXT("Wall")) && Hit.GetActor()->ActorHasTag(TEXT("KillZone")))
	{
		// @TODO: End the round, show the end widget
		return;
	}

	Direction = GetActorForwardVector();
	FVector Reflected = FMath::GetReflectionVector(Direction, Hit.ImpactNormal).GetSafeNormal2D();
	double Angle = FMath::RadiansToDegrees(FMath::Atan2(Reflected.Y, Reflected.X));

	// Handle paddle velocity influence on this ball
	// @TODO: Creat a point system and increase or decrease the points based on paddle presence 
	if (APaddleBase* Paddle = Cast<APaddleBase>(Hit.GetActor()))
	{
		float PaddleVelocity;
		if (Paddle->TryGetVelocity(PaddleVelocity))
		{
			Angle += PaddleVelocity * FMath::RandRange(1.0f, MaxPaddleAngleInfluence);
		}
	}

	Angle = (Reflected.X > 0) ? FMath::ClampAngle(Angle, -MaxBounceAngle.X, MaxBounceAngle.X) : FMath::ClampAngle(Angle, -MaxBounceAngle.Y, MaxBounceAngle.Y);
	SetActorRotation(FRotator(0.0f, Angle, 90.0f));
	UpdateDirection();

	UE_LOG(LogPongBall, Verbose, TEXT("HandleBallHit: New Angle: %f"), Angle)

}
