// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "BasicEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UBasicEnemyController : public UOrbitObjectControllerBase
{
	GENERATED_BODY()

	// Members ************************************************************************************

	// Methods ************************************************************************************
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHitCallback() override;

	virtual void Init(AOrbitObject* obj);
};
