// Copyright (c) 2026, Darshit Limbad

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPongBall, Log, All)

class UPrimitiveComponent;
class UArrowComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EBallDirection : uint8
{
	None = 0,
	Left,
	Right
};

UCLASS(BlueprintType, Blueprintable, category=PongBall)
class PINGPONGGAME_API APongBall : public AActor
{
	GENERATED_BODY()
public:
	APongBall();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	

public:

	// Public APIs
	
	inline const float GetCurrentVelocity() const { return Velocity; };
	inline const float GetYCoordinate() const { return GetActorLocation().Y; };

protected:

	/** Moves the ball everyframe */
	void MoveBall(float DeltaSeconds);
	void UpdateDirection();
	void SetHorizontalDirection(EBallDirection NewDirection = EBallDirection::None);
	EBallDirection GetHorizontalDirection() { return HorizontalDirection; };

private:

	/** Sphere collison to detect hit and overlap events */
	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	TObjectPtr<USphereComponent> SphereComponent;

	/** Sprite asset to render */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSpriteComponent> RenderComponent;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing. */
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif

	/** Starting speed of the pong */
	UPROPERTY(EditAnywhere, Category = "PongBall|Speed")
	float StartSpeed = 250.0f;

	/** Max speed of the pong, will gradually keep increasing. */
	UPROPERTY(EditAnywhere, Category = "PongBall|Speed", meta= (ClampMin = "10"))
	float MaxSpeed =  500.0f;

	UPROPERTY(EditAnywhere, Category = "PongBall|Rotation", meta= (ClampMin= "1", ClampMax= "25"))
	float MaxPaddleAngleInfluence = 10.0f;

	/** Maximum reflection angles for the ball movement.
	*	X = Left Side Angle
	*	Y = Right Side Angle
	*/
	UPROPERTY(EditAnywhere, Category = "PongBall|Rotation|Advanced|Angle")
	FVector2D MaxBounceAngle = {70.f, 140.f};

	float Velocity;
	FVector Direction;
	EBallDirection HorizontalDirection = EBallDirection::None;

	UPROPERTY(EditAnywhere, Category = "PongBall|Debug")
	uint8 bDebugMode:1;

private:
	void HandleBallHit(const FHitResult& Hit);
};

