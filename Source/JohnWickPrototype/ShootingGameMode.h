// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TargetableComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JOHNWICKPROTOTYPE_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShootingGameMode();

	void AddTargetableComponentToList(UTargetableComponent* component);
	void RemoveTargetableComponentFromList(UTargetableComponent* component);

	const TArray<UTargetableComponent*>& GetTargetableComponents() { return mTargetableComponents; }

private:
	TArray<UTargetableComponent*> mTargetableComponents;
};
