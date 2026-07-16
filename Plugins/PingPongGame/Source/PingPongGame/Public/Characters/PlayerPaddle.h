// Copyright (c) 2026, Darshit Limbad

#pragma once 

#include "CoreMinimal.h"
#include "Characters/PaddleBase.h"
#include "PlayerPaddle.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerPaddle, Log, All)

class UInputMappingContext;
class UInputAction;

UCLASS(Abstract, Blueprintable, BlueprintType, category = Character, meta=(DisableNativeTick))
class APlayerPaddle : public APaddleBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlay) override;

protected:

	// Default Input Mapping context
	UPROPERTY(EditDefaultsOnly, category = "Paddle|Input")
	TObjectPtr<UInputMappingContext> IMC;

	// Movement input action for paddle up-down movement.
	UPROPERTY(EditDefaultsOnly, category = "Paddle|Input")
	TObjectPtr<UInputAction> MovementIA;

private:
	void HandleMoveUp(const struct FInputActionValue& Value);
};

