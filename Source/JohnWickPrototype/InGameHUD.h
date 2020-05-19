// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class JOHNWICKPROTOTYPE_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AInGameHUD();

	virtual void DrawHUD() override;
	
protected:
	virtual void BeginPlay() override;

private:
	void DrawRadialLine(float angle, float start, float length, float thickness, FLinearColor colour);
	void DrawCircle(float centreX, float centreY, float radius);

private:
	class AShootingGameMode* mGameMode;

	float mScreenCentreX;
	float mScreenCentreY;

	class UTexture2D* mCircleTexture;
	int mCircleWidth;
};
