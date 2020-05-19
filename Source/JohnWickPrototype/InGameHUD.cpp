// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameHUD.h"

#include "Engine/Canvas.h"
#include "TargetableComponent.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

#include "ShootingGameMode.h"
#include "MainCharacterController.h"

AInGameHUD::AInGameHUD() :
	mCircleTexture(nullptr)
,	mCircleWidth(0)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> circleFinder(TEXT("/Game/Images/HudCircle.HUDCircle"));

	if (circleFinder.Succeeded())
	{
		mCircleTexture = circleFinder.Object;
		mCircleWidth = mCircleTexture->GetSurfaceWidth();
	}
}

void AInGameHUD::BeginPlay()
{
	mGameMode = Cast<AShootingGameMode>(GetWorld()->GetAuthGameMode());
}

void AInGameHUD::DrawHUD()
{
	Canvas->GetCenter(mScreenCentreX, mScreenCentreY);

	AMainCharacterController* mainCharacterController = Cast<AMainCharacterController>(GetOwningPlayerController());

	if (mainCharacterController->IsAiming())
	{
		DrawCircle(mScreenCentreX, mScreenCentreY, 45);

		FVector2D aimOffset = mainCharacterController->GetAimOffset();

		if (!aimOffset.IsZero())
		{
			const float aimTolerance = 10 * PI / 180;
			float aimAngle = mainCharacterController->GetAimAngle();
			DrawRadialLine(aimAngle, 0, aimOffset.Size(), 3.0f, FLinearColor::Red);
			DrawRadialLine(aimAngle + aimTolerance, 165, 50, 1.0f, FLinearColor::Yellow);
			DrawRadialLine(aimAngle - aimTolerance, 165, 50, 1.0f, FLinearColor::Yellow);
		}
	}
	else if (mainCharacterController->IsAimed())
	{
		FVector2D aimOffset = mainCharacterController->GetAimOffset();

		DrawCircle(mScreenCentreX + aimOffset.X, mScreenCentreY + aimOffset.Y, 8);
		DrawCircle(mScreenCentreX, mScreenCentreY, mainCharacterController->GetMaxAimedOffset());
	}

	const TArray<UTargetableComponent*>& targetableComps = mGameMode->GetTargetableComponents();
	for (auto targetableComp : targetableComps)
	{
		float drawAngle = targetableComp->GetYawRelativeToPlayer();
		DrawRadialLine(drawAngle, 180, 20, 3.0f, FLinearColor::White);
	}

	AHUD::DrawHUD();
}

void AInGameHUD::DrawRadialLine(float angle, float start, 
	float length, float thickness, FLinearColor colour)
{
	float lineFinish = start + length;

	DrawLine(
		mScreenCentreX + start * FMath::Sin(angle),
		mScreenCentreY - start * FMath::Cos(angle),
		mScreenCentreX + lineFinish * FMath::Sin(angle),
		mScreenCentreY - lineFinish * FMath::Cos(angle),
		colour, thickness);
}

void AInGameHUD::DrawCircle(float centreX, float centreY, float radius)
{
	if (mCircleTexture != nullptr)
	{
		const float scale = 2 * radius / mCircleWidth;
		DrawTextureSimple(mCircleTexture,
			centreX - radius,
			centreY - radius,
			scale);
	}
}
