// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbitObjectControllers/BasicEnemyController.h"
#include "HealthObjectController.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UHealthObjectController : public UBasicEnemyController
{
	GENERATED_BODY()
	// Members ************************************************************************************


	// Methods ************************************************************************************
public:
	UHealthObjectController();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Init(AOrbitObject* obj, int HP) override;

	virtual void OnHitCallback() override;


	static AOrbitObject* SpawnHealthObject(AActor* actor, ASphereWorld* sWorld);
};
