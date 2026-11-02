// Copyright (c) 2026, Darshit Limbad

#include "Actors/PongBall.h"
#include "Characters/PaddleBase.h"

#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"

DEFINE_LOG_CATEGORY(LogPongBall)

APongBall::APongBall()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->InitSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

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
	
	FVector Forward = GetActorForwardVector();
	Direction = FVector(Forward.X, Forward.Y, 0.0f);
	
	Velocity = StartSpeed;

	SphereComponent->OnComponentHit.AddDynamic(this, &APongBall::HandleOnSphereHit);
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
	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * Velocity * DeltaSeconds;
	SetActorLocation(Location, true);

	if (Velocity <= MaxSpeed)
	{
		Velocity += 1.0f;
	}
}

void APongBall::HandleOnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(TEXT("Wall")) && OtherActor->ActorHasTag(TEXT("KillZone")))
	{
		// @TODO: End the round, show the end widget
		return;
	}

	Direction = GetActorForwardVector();
	FVector Reflected = FMath::GetReflectionVector(Direction, Hit.ImpactNormal).GetSafeNormal2D();
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(Reflected.Y, Reflected.X));
	
	// Handle paddle velocity influence on this ball
	APaddleBase* Paddle = Cast<APaddleBase>(OtherActor);
	// @todo: Creat a point system and increase or decrease the points based on paddle presence 
	if (Paddle)
	{
		float PaddleVelocity;
		if (Paddle->TryGetVelocity(PaddleVelocity))
		{
			Angle += PaddleVelocity * FMath::RandRange(5.0f, MaxPaddleVelocityAngleInfluence);
		}
	}

	Angle = (Reflected.X > 0) ? FMath::ClampAngle(Angle, -LeftSideAngle, LeftSideAngle) : FMath::ClampAngle(Angle, -RightSideAngle, RightSideAngle);
	Direction.X = FMath::Cos(FMath::DegreesToRadians(Angle));
	Direction.Y = FMath::Sin(FMath::DegreesToRadians(Angle));

	SetActorRotation(FRotator(0.0f, Angle, 90.0f));

	UE_LOG(LogPongBall, Verbose, TEXT("New Direction: X: %f Y: %f New Angle: %f"), Direction.X, Direction.Y, Angle)
}
