// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbitObject.h"
#include "SphereWorldGameState.generated.h"

class ASphereWorld;
class ACustomARPawn;

enum class ARGameStates
{
	Main_Menu,
	Gameplay,
	Options_Menu,
	Help_Menu,
	Death_menu
};

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

	ASphereWorld* CreateSphereWorld(FVector worldPosition, FTransform trans);

	void SpawnEnemy();
	void SpawnControllerEnemy();

	inline ARGameStates GetGameState() const { return m_gameState;}
	inline void SetGameState(const ARGameStates& state) {m_gameState = state;}

	void BeginGame();

	void SetPawn(ACustomARPawn* pawn);
	ACustomARPawn* GetPawn();

protected:

	ASphereWorld* m_sphereWorld;

	TArray<AOrbitObject*> m_enemies;

	ARGameStates m_gameState;

	ACustomARPawn* m_pawn;
};
