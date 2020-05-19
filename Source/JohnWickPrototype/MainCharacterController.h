// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class JOHNWICKPROTOTYPE_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainCharacterController();

private:
	void UpdateTargetables();
	void AimAtTarget();

	static float GetAngle(const FVector2D& vector);

private:
	// Cached values
	class AShootingGameMode* mGameMode;

	class AMainCharacter* mCharacter;

	int32 mScreenWidth;
	int32 mScreenHeight;
	float mHorizFOV;
	float mVertFOV;

	// Input
	FVector2D mMovementInput;
	FVector2D mMouseInput;

	// Movement
	FVector mNextPosition;

	// Aiming
	bool mIsAiming;
	bool mIsAimed;
	class UTargetableComponent* mAimedTarget;
	int mUpdateAimCounter;

	FVector2D mAimOffset;
	float mAimAngle;

	float mMaxAimedOffset;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	const FVector2D& GetAimOffset() const { return mAimOffset; }
	float GetAimAngle() const { return mAimAngle; }

	float GetMaxAimedOffset() const { return mMaxAimedOffset; }

	bool IsAiming() const { return mIsAiming; }
	bool IsAimed() const { return mIsAimed; }

	void MoveForward(float amount);
	void MoveRight(float amount);

	void MouseRight(float amount);
	void MouseUp(float amount);

	void Aim();
	void EndAim();

	void Shoot();
};
