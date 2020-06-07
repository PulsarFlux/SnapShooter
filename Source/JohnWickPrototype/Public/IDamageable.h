// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Interface for damageable actor
 */
class JOHNWICKPROTOTYPE_API IDamageable
{
public:
	IDamageable();
	~IDamageable();

public:
	virtual void GotHit() = 0;
};
