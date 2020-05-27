// Fill out your copyright notice in the Description page of Project Settings.

#include "BobbingTarget.h"

// Sets default values
ABobbingTarget::ABobbingTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABobbingTarget::BeginPlay()
{
	Super::BeginPlay();

	mLastTime = FMath::FRand() * 2 * PI;
}

// Called every frame
void ABobbingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mLastTime += DeltaTime;
	if (mLastTime > 2 * PI) { mLastTime -= 2 * PI; }
	AddActorWorldOffset(FVector::UpVector * 2 * FMath::Sin(mLastTime));
}

