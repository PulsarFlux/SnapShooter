// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageableComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UDamageableComponent::UDamageableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	miHealth = 50;
	mbIsDead = false;
}


// Called every frame
void UDamageableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageableComponent::GotHit()
{
	mParent->GotHit();
	miHealth -= 10;
	if (miHealth <= 0)
	{
		GetOwner()->Destroy();
		mbIsDead = true;
	}
}