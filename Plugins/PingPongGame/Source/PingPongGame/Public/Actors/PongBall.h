// Copyright (c) 2026, Darshit Limbad

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPongBall, Log, All)

class UPrimitiveComponent;
class UArrowComponent;
class USphereComponent;

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

private:

	/** Sphere collison to detect hit and overlap events */
	UPROPERTY(VisibleAnywhere, Category = Collision)
	TObjectPtr<USphereComponent> SphereComponent;

	/** Sprite asset to render */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSpriteComponent> RenderComponent;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing. */
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif

	/** Starting speed of the pong */
	UPROPERTY(EditDefaultsOnly, Category = "PongBall|Speed")
	float StartSpeed = 10.0f;

	/** Max speed of the pong, will gradually keep increasing. */
	UPROPERTY(EditAnywhere, Category = "PongBall|Speed", meta= (ClampMin = "10"))
	float MaxSpeed =  20.0f;

	UPROPERTY(EditAnywhere, Category = "PongBall|Rotation", meta= (ClampMin= "10", ClampMax= "25"))
	float MaxPaddleVelocityAngleInfluence = 20.0f;

	UPROPERTY(EditAnywhere, Category = "PongBall|Rotation|Advanced|")



	/** The Effective Speed of the ball, this will keep increasing until it reaches max speed. */
	float Velocity;

	/** ths actor's normalized directon, 
	*	Since UE supports 3D for most computation we would use 3D as 2D vector
	*	so that we don't have to convert 2D to 3D while computing reflection. 
	*/
	FVector Direction;

private:

	/** Handles Sphere hit events */
	UFUNCTION()
	void HandleOnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

