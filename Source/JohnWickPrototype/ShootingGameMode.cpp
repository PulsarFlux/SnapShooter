// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootingGameMode.h"
#include "MainCharacter.h"
#include "MainCharacterController.h"
#include "InGameHUD.h"


AShootingGameMode::AShootingGameMode()
{
	DefaultPawnClass = AMainCharacter::StaticClass();

	PlayerControllerClass = AMainCharacterController::StaticClass();

	HUDClass = AInGameHUD::StaticClass();
}

void AShootingGameMode::AddTargetableComponentToList(UTargetableComponent* component)
{
	mTargetableComponents.Add(component);
}