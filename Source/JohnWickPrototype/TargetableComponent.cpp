// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetableComponent.h"

#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTargetableComponent::UTargetableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTargetableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogTemp, Log, TEXT(__FUNCTION__));
	
	Cast<AShootingGameMode>(GetWorld()->GetAuthGameMode())->AddTargetableComponentToList(this);
}


// Called every frame
void UTargetableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetableComponent::Update(const FVector& playerPos, const FVector& playerFacing)
{
	float playerYaw = playerFacing.ToOrientationRotator().Yaw;
	FVector pointToTarget = GetOwner()->GetActorLocation() - playerPos;
	float targetYaw = pointToTarget.ToOrientationRotator().Yaw;
	mRelativeYaw = PI * (targetYaw - playerYaw) / 180;
}

void UTargetableComponent::OnComponentDestroyed(bool bDestroyingHeirarchy)
{
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AShootingGameMode* gameMode = Cast<AShootingGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->RemoveTargetableComponentFromList(this);
		}
	}
}

