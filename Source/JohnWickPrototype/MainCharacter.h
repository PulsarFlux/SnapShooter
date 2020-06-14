// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class JOHNWICKPROTOTYPE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	class USpringArmComponent* mCameraSpringArm;
	class UCameraComponent* mCameraComponent;

	class AShootingGameMode* mGameMode;
	class AMainCharacterController* mController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float SetAim(FVector relativeVector);
	void  AddAimPitchDelta(float pitchDelta);

	float GetCameraAngle() { return mCameraPitch + mCameraOffsetAngle; }

	float GetShoulderAimOffset() const { return mShoulderAimOffset; }

	FVector GetCameraPos();

	// Horizontal, in degrees
	float GetFOV() const;
	float GetAspectRatio() const;

private:
	float mCameraPitch;
	// Angle between camera view direction and spring arm.
	UPROPERTY(EditAnywhere)
	float mCameraOffsetAngle = 30.0f;
	UPROPERTY(EditAnywhere)
	float mSpringArmLength = 250.0f;
	UPROPERTY(EditAnywhere)
	float mShoulderAimOffset = 30.0f;
};
