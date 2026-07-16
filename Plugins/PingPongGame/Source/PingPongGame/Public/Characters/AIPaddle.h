// Copyright (c) 2026, Darshit Limbad

#pragma once

#include "CoreMinimal.h"
#include "Characters/PaddleBase.h"
#include "AIPaddle.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIPaddle, Log, All)

struct FAIPaddleDifficulty
{
	float ReactionTime;	
	float DeadZone;
	float PaddleSpeed;
};

UENUM(BlueprintType)
enum class EAIPaddleDifficulty : uint8
{
	Easy    UMETA(DisplayName = "Easy"),
	Medium  UMETA(DisplayName = "Medium"),
	Hard    UMETA(DisplayName = "Hard")
};

class APongBall;

UCLASS(BlueprintType, Blueprintable, category = Character, meta = (PrioritizeCategories = "Paddle"))
class PINGPONGGAME_API AAIPaddle : public APaddleBase
{
	GENERATED_BODY()

public:
	AAIPaddle();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:

	/** 
	* Reference to the ball that this actor tracks. 
	* If let unset, the actor will automatically find an APongBall during BeginPlay().
	*/
	UPROPERTY(EditAnywhere, Category = "Paddle|AI")
	TObjectPtr<APongBall> Ball;

	/** Ai difficulty mode */
	UPROPERTY(EditAnywhere, Category = "Paddle|AI")
	EAIPaddleDifficulty Difficulty = EAIPaddleDifficulty::Medium;

	UPROPERTY(EditAnywhere, Category = "Paddle|AI")
	float ReactionTime = 0.20f;

	UPROPERTY(EditAnywhere, Category = "Paddle|AI")
	float DeadZone = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Paddle|AI")
	float PaddleSpeed = 200.0f;

private:

	FAIPaddleDifficulty DifficultyData;
};