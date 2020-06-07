// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/IDamageable.h"
#include "BobbingTarget.generated.h"

UCLASS()
class JOHNWICKPROTOTYPE_API ABobbingTarget : public AActor, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABobbingTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GotHit() override;

	UPROPERTY(EditAnywhere)
	class UMaterial* mDefaultMaterial;
	UPROPERTY(EditAnywhere)
	class UMaterial* mTookDamageMaterial;
private:
	float mLastTime;

	float mDisplayDamageTimer;

	class UStaticMeshComponent* mSphereMesh;
};
