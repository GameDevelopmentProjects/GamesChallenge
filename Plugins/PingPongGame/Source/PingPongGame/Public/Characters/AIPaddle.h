// Copyright (c) 2026, Darshit Limbad

#pragma once

#include "CoreMinimal.h"
#include "Characters/PaddleBase.h"
#include "AIPaddle.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIPaddle, Log, All)

USTRUCT(BlueprintType)
struct FAIPaddleDifficulty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Difficulty")
	float PaddleSpeed;
};

UENUM(BlueprintType)
enum class EAIPaddleDifficulty : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Default  UMETA(DisplayName = "Default"),
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
	EAIPaddleDifficulty Difficulty = EAIPaddleDifficulty::Default;

	UPROPERTY(EditAnywhere, Category = "Paddle|AI|Difficulty")
	FAIPaddleDifficulty EasyDifficultyData = { 0.3f };

	UPROPERTY(EditAnywhere, Category = "Paddle|AI|Difficulty")
	FAIPaddleDifficulty DefaultDifficultyData = { 0.1f };

	UPROPERTY(EditAnywhere, Category = "Paddle|AI|Difficulty")
	FAIPaddleDifficulty HardDifficultyData = { 0.0f };

	UPROPERTY(EditAnywhere, Category = "Paddle|Debug")
	uint8 bDebugMode:1;

private:

	FAIPaddleDifficulty DifficultyData = DefaultDifficultyData;
};