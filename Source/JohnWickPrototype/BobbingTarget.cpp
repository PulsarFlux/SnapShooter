// Fill out your copyright notice in the Description page of Project Settings.

#include "BobbingTarget.h"

#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "DamageableComponent.h"

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

	mDisplayDamageTimer = 0.0f;

	UActorComponent* actorComp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	mSphereMesh = static_cast<UStaticMeshComponent*>(actorComp);

	actorComp = GetComponentByClass(UDamageableComponent::StaticClass());
	UDamageableComponent* dmgComp = static_cast<UDamageableComponent*>(actorComp);
	dmgComp->SetParent(this);
}

// Called every frame
void ABobbingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mLastTime += DeltaTime;
	if (mLastTime > 2 * PI) { mLastTime -= 2 * PI; }
	AddActorWorldOffset(FVector::UpVector * 2 * FMath::Sin(mLastTime));

	if (mDisplayDamageTimer > 0.0f)
	{
		mDisplayDamageTimer -= DeltaTime;
		if (mDisplayDamageTimer <= 0.0f)
		{
			mSphereMesh->SetMaterial(0, mDefaultMaterial);
		}
	}
}

void ABobbingTarget::GotHit()
{
	static const float kDisplayDamageMaterialTime = 0.5f;
	mSphereMesh->SetMaterial(0, mTookDamageMaterial);
	mDisplayDamageTimer = kDisplayDamageMaterialTime;
}


