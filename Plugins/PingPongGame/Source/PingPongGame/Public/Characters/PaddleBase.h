// Copyright (c) 2026, Darshit Limbad

#pragma once 

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaddleBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaddleBase, Log, All)

/** A base class which provides internal methods like move up. */
UCLASS(Abstract, Blueprintable, BlueprintType, category = Character, meta= (DisableNativeTick, PrioritizeCategories= "Paddle"))
class PINGPONGGAME_API APaddleBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	APaddleBase();

	/** true if paddle is moving, false otherwise */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Paddle|Movement")
	inline bool GetIsMoving() const { return bIsMoving; };

	/** returns current velocity of the paddle */
	inline float GetVelocityValue() const { return Velocity; };

	/** if the paddle is moving it would out velocity of the paddle and return true, false otherwise */
	bool TryGetVelocity(float& OutVelocity) const;

protected:

	// Returns this characters movement speed multiplier float.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Paddle|Input")
	inline float GetMovementSpeedMultiplier() { return MovementSpeedMultiplier; };

	// Movement related private APIs
	void NotifyMovementStarted();
	void MoveUp(float MoveInput);
	void NotifyMovementEnded();

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMovementSpeedMultiplier, Category = "Paddle|Input", meta = (ClampMin = "0.5", UIMin = "0.5"))
	float MovementSpeedMultiplier = 1.0f;

private:
	

	float Velocity;
	bool bIsMoving = false;
};