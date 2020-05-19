// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "MainCharacterController.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "ShootingGameMode.h"
#include "TargetableComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	RootComponent = GetCapsuleComponent();
	
	//GetCapsuleComponent()->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshFinder(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	
	if (meshFinder.Succeeded())
	{
		//GetMesh()->SetSkeletalMesh(meshFinder.Object);
	}

	//mCameraOffsetAngle = 30.0f;
	//mSpringArmLength = 200.0f;

	mCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	mCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	mCameraSpringArm->bEnableCameraLag = false;
	//->CameraLagSpeed = 3.0f;
	mCameraSpringArm->bInheritYaw = true;
	mCameraSpringArm->bInheritPitch = true;


	mCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	mCameraComponent->SetupAttachment(mCameraSpringArm, USpringArmComponent::SocketName);
	mCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(mCameraOffsetAngle, 0.0f, 0.0f));
	mCameraComponent->bUsePawnControlRotation = false;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	UE_LOG(LogTemp, Warning, TEXT("Constructor"));
}

// In degrees
float AMainCharacter::GetFOV() const
{
	return mCameraComponent->FieldOfView;
}
float AMainCharacter::GetAspectRatio() const 
{
	return mCameraComponent->AspectRatio;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	mCameraPitch = 0.0f;

	mCameraSpringArm->TargetArmLength = mSpringArmLength;
	mCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, GetShoulderAimOffset(), 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	mGameMode = Cast<AShootingGameMode>(GetWorld()->GetAuthGameMode());

	// mHUDWidget->AddToViewport();

	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

	mController = GetController<AMainCharacterController>();

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis<AMainCharacterController>("MoveForward", mController, &AMainCharacterController::MoveForward);
	PlayerInputComponent->BindAxis<AMainCharacterController>("MoveRight", mController, &AMainCharacterController::MoveRight);

	PlayerInputComponent->BindAxis<AMainCharacterController>("Turn", mController, &AMainCharacterController::MouseRight);
	PlayerInputComponent->BindAxis("LookUp", mController, &AMainCharacterController::MouseUp);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, mController, &AMainCharacterController::Aim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, mController, &AMainCharacterController::EndAim);

	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, mController, &AMainCharacterController::Shoot);
}


float AMainCharacter::SetAim(FVector relativeVector)
{
	const float targetDistance = relativeVector.Size();
	float armLength = mSpringArmLength;
	FVector cameraVector = mCameraSpringArm->GetSocketLocation(USpringArmComponent::SocketName) - GetActorLocation();
	if (mCameraSpringArm->IsCollisionFixApplied())
	{
		armLength = cameraVector.Size();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Arm length 1 %f"), mSpringArmLength);
	//UE_LOG(LogTemp, Warning, TEXT("Arm length 2 %f"), armLength);

	// Angle between the relative vector and the target-to-camera vector (in degrees)
	float theta = (180 / PI) * FMath::Asin(armLength * FMath::Sin((PI / 180) * mCameraOffsetAngle) / targetDistance);
	// Angle between the relative vector and the horizontal (in degrees)
	float phi = (180 / PI) * FMath::Asin(relativeVector.Z / targetDistance);

	float pitch = -1 * (mCameraOffsetAngle + theta - phi);

	mCameraSpringArm->SetRelativeLocationAndRotation(
		FVector(0.0f, GetShoulderAimOffset(), 0.0f),
		FRotator(pitch, 0.0f, 0.0f));

	mCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(mCameraOffsetAngle, 0.0f, 0.0f));

	mCameraPitch = pitch;

	// return the distance between the camera and the target
	return (relativeVector - cameraVector).Size();
}

void AMainCharacter::AddAimPitchDelta(float pitchDelta)
{
	mCameraPitch += pitchDelta;

	mCameraSpringArm->SetRelativeLocationAndRotation(
		FVector(0.0f, GetShoulderAimOffset(), 0.0f),
		FRotator(mCameraPitch, 0.0f, 0.0f));

	mCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(mCameraOffsetAngle, 0.0f, 0.0f));
}

	/*
	UE_LOG(LogTemp, Log, TEXT(__FUNCTION__));

	float minDist = 100000;
	UTargetableComponent* closestTarget = nullptr;

	for (auto comp : mGameMode->GetTargetableComponents())
	{
		float compDist = FVector::Distance(comp->GetOwner()->GetActorLocation(), GetActorLocation());
		if (compDist < minDist)
		{
			minDist = compDist;
			closestTarget = comp;
		}
	}

	if (closestTarget != nullptr)
	{
		FVector aimFromPos = GetActorLocation();
		aimFromPos.Z += 100;
		FVector pointToTarget = closestTarget->GetOwner()->GetActorLocation() - aimFromPos;
		FVector pointAway = -1 * pointToTarget;

		// mCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), pointToTarget.ToOrientationRotator());

		FRotator currentAngles = GetControlRotation();
		FRotator targetAngles = pointToTarget.ToOrientationRotator();

		UE_LOG(LogTemp, Log, TEXT("target yaw: %f"), targetAngles.Yaw);
		UE_LOG(LogTemp, Log, TEXT("current yaw: %f"), currentAngles.Yaw);
		UE_LOG(LogTemp, Log, TEXT("yaw diff: %f"), targetAngles.Yaw - currentAngles.Yaw);
		AddControllerYawInput(targetAngles.Yaw - currentAngles.Yaw);

		mCameraPitch = targetAngles.Pitch;

		// SetActorRotation(pointToTarget.ToOrientationQuat());
	}
}

*/