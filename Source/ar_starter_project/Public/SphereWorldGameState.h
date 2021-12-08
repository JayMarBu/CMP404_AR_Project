// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomActor.h"
#include "SphereWorldGameState.generated.h"

class ASphereWorld;

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API ASphereWorldGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASphereWorldGameState();

	inline ASphereWorld* GetSphereWorld() const { return m_sphereWorld; }
	inline void SetSphereWorld(ASphereWorld* newSphereWorld) { m_sphereWorld = newSphereWorld; }

	ASphereWorld* CreateSphereWorld(FVector worldPosition);

	void SpawnEnemy();

protected:

	ASphereWorld* m_sphereWorld;

	TArray<ACustomActor*> m_enemies;
};