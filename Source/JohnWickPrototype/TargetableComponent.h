// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOHNWICKPROTOTYPE_API UTargetableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Update(const FVector& playerPos, const FVector& playerFacing);

	// Ranges from -2 * PI to 2 * PI
	float GetYawRelativeToPlayer() const { return mRelativeYaw; }

	float GetTargetSize() const { return mTargetSize; }

private:
	float mRelativeYaw;
	UPROPERTY(EditAnywhere)
	float mTargetSize = 10;
};