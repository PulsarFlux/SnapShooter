// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterController.h"

#include "Engine.h"
#include "Engine/Blueprint.h"

#include "DrawDebugHelpers.h"

#include "ShootingGameMode.h"
#include "TargetableComponent.h"
#include "MainCharacter.h"

AMainCharacterController::AMainCharacterController()
{
	//InputYawScale = 1.0f;
}

void AMainCharacterController::BeginPlay()
{
	mGameMode = Cast<AShootingGameMode>(GetWorld()->GetAuthGameMode());

	mCharacter = Cast<AMainCharacter>(GetPawn());

	GetViewportSize(mScreenWidth, mScreenHeight);

	mHorizFOV = mCharacter->GetFOV();
	mVertFOV = mHorizFOV / mCharacter->GetAspectRatio();

	UE_LOG(LogTemp, Log, TEXT("size %dx%d, fov h %f v %f"), mScreenWidth, mScreenHeight, mHorizFOV, mVertFOV);
}

void AMainCharacterController::Tick(float DeltaTime)
{
	if (!mMovementInput.IsZero())
	{
		//Scale our movement input axis values by 400 units per second
		mMovementInput = mMovementInput.GetSafeNormal() * 400.0f;
		mNextPosition = mCharacter->GetActorLocation();
		mNextPosition += mCharacter->GetActorForwardVector() * mMovementInput.X * DeltaTime;
		mNextPosition += mCharacter->GetActorRightVector() * mMovementInput.Y * DeltaTime;
		//UE_LOG(LogTemp, Log, TEXT("next pos: %f %f %f"), mNextPosition.X, mNextPosition.Y, mNextPosition.Z);
		mCharacter->SetActorLocation(mNextPosition);
		FVector actualPos = mCharacter->GetActorLocation();
		//UE_LOG(LogTemp, Log, TEXT("actual pos: %f %f %f"), actualPos.X, actualPos.Y, actualPos.Z);
	}

	UpdateTargetables();

	if (IsAiming())
	{
		mAimOffset.X += 4 * mMouseInput.X;
		mAimOffset.Y += -4 * mMouseInput.Y;

		const float kMaxAimOffset = 50;
		float aimOffsetLength = mAimOffset.Size();
		if (aimOffsetLength > kMaxAimOffset)
		{
			mAimOffset *= kMaxAimOffset / aimOffsetLength;
		}

		mAimAngle = GetAngle(mAimOffset);
	}
	else if (IsAimed())
	{
		AimAtTarget();

		mAimOffset.X += 8 * mMouseInput.X;
		mAimOffset.Y += 8 * mMouseInput.Y;

		const float maxOffset = GetMaxAimedOffset();
		if (mAimOffset.Size() > maxOffset)
		{
			mAimOffset.Normalize();
			mAimOffset *= maxOffset;
		}
	}
	else
	{
		ControlRotation.Yaw += 2 * mMouseInput.X;
		mCharacter->AddAimPitchDelta(-2 * mMouseInput.Y);
	}

	mMovementInput = { 0 , 0 };
	mMouseInput = { 0 , 0 };
}

void AMainCharacterController::MoveForward(float amount)
{
	mMovementInput.X = amount;
}

void AMainCharacterController::MoveRight(float amount)
{
	mMovementInput.Y = amount;
}

void AMainCharacterController::MouseRight(float amount)
{
	mMouseInput.X = amount;
}

void AMainCharacterController::MouseUp(float amount)
{
	mMouseInput.Y = amount;
}

void AMainCharacterController::Aim()
{
	mAimOffset = FVector2D::ZeroVector;
	mIsAiming = true;
	mIsAimed = false;
}

void AMainCharacterController::EndAim()
{
	mIsAiming = false;

	if (mAimOffset.Size() > 45)
	{
		float minAimDiff = 2 * PI;
		UTargetableComponent* aimedTarget = nullptr;
		for (auto target : mGameMode->GetTargetableComponents())
		{
			float relativeYaw = target->GetYawRelativeToPlayer();
			// Convert into -PI to +PI range.
			float relativeAngle = relativeYaw > PI ? -2 * PI + relativeYaw :
				relativeYaw < -1 * PI ? 2 * PI + relativeYaw : relativeYaw;

			float aimDiff = FMath::Abs(relativeAngle - mAimAngle);
			if (aimDiff < minAimDiff)
			{
				minAimDiff = aimDiff;
				aimedTarget = target;
			}
		}

		mAimedTarget = aimedTarget;

		if (aimedTarget != nullptr)
		{
			mIsAimed = true;
			UE_LOG(LogTemp, Log, TEXT("rel yaw before: %f"), mAimedTarget->GetYawRelativeToPlayer());
			AimAtTarget();
			UpdateTargetables();
			UE_LOG(LogTemp, Log, TEXT("rel yaw after: %f"), mAimedTarget->GetYawRelativeToPlayer());
			mUpdateAimCounter = 0;
			mAimOffset = FVector2D::ZeroVector;
		}
	}
}

void AMainCharacterController::Shoot()
{
	if (IsAimed())
	{
		FVector aimFromPos = mCharacter->GetActorLocation();
		aimFromPos += mCharacter->GetShoulderAimOffset() * mCharacter->GetActorRightVector();

		float rayLength = 3 * FVector::Dist(mAimedTarget->GetOwner()->GetActorLocation(), aimFromPos);

		FRotator aimRot = ControlRotation;

		aimRot.Pitch = mCharacter->GetCameraAngle();

		UE_LOG(LogTemp, Log, TEXT("aimRot: %s"), *(aimRot.ToString()));

		float offsetScreenFracX = mAimOffset.X / mScreenWidth;
		float offsetScreenFracY = mAimOffset.Y / mScreenHeight;

		// Temp angle calc
		// TODO - Proper calc using tan etc
		float offsetXAngle = mHorizFOV * offsetScreenFracX;
		float offsetYAngle = mVertFOV * offsetScreenFracY;

		aimRot.Pitch -= offsetYAngle;
		aimRot.Yaw += offsetXAngle;

		UE_LOG(LogTemp, Log, TEXT("aimRot: %s"), *(aimRot.ToString()));

		FVector aimEnd = aimFromPos + rayLength * aimRot.Vector();

		DrawDebugLine
		(
			UObject::GetWorld(),
			aimFromPos,
			aimEnd,
			FColor{ 255, 0, 0 },
			false,
			1.0f,
			0,
			1.0f
		);

/*
			const UWorld * InWorld,
			FVector const & LineStart,
			FVector const & LineEnd,
			FColor const & Color,
			bool bPersistentLines,
			float LifeTime,
			uint8 DepthPriority,
			float Thickness
		)*/

		FHitResult hitResult;
		FCollisionObjectQueryParams objectQueryParams
			{ ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic) };
		FCollisionQueryParams queryParams{};

		UObject::GetWorld()->LineTraceSingleByObjectType(
			hitResult, aimFromPos, aimEnd,
			objectQueryParams, queryParams);

		UE_LOG(LogTemp, Log, TEXT("shoot hit result: %s"), *(hitResult.ToString()));
	}
}

void AMainCharacterController::UpdateTargetables()
{
	FVector ourPosition = mCharacter->GetActorLocation();
	FVector forwardVector = mCharacter->GetActorForwardVector();
	for (auto target : mGameMode->GetTargetableComponents())
	{
		target->Update(ourPosition, forwardVector);
	}
}

void AMainCharacterController::AimAtTarget()
{
	FVector aimFromPos = mCharacter->GetActorLocation();
	aimFromPos += mCharacter->GetShoulderAimOffset() * mCharacter->GetActorRightVector();
	FVector pointToTarget = mAimedTarget->GetOwner()->GetActorLocation() - aimFromPos;

	FRotator targetAngles = pointToTarget.ToOrientationRotator();

	float camTargetDist = mCharacter->SetAim(pointToTarget);

	mMaxAimedOffset = 5 * mAimedTarget->GetTargetSize() * 1000 / camTargetDist;

	targetAngles.Pitch = 0;

	mCharacter->SetActorRotation(targetAngles);
	ControlRotation = targetAngles;
}


float AMainCharacterController::GetAngle(const FVector2D& vector)
{
	float result = 0;
	if (vector.Y == 0)
	{
		result = vector.X < 0 ? -1 * PI / 2 : PI / 2;
	}
	else
	{
		result = FMath::Atan2(vector.X, vector.Y);
	}
	return result;
}