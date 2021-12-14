// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbitObject.h"
#include "SphereWorldGameState.generated.h"

class ASphereWorld;
class ACustomARPawn;
class AMainMenuHud;
class WaveSpawner;

enum class ARGameStates
{
	Main_Menu,
	Gameplay,
	Options_Menu,
	Help_Menu,
	Death_menu
};

enum class EnemyType
{
	Basic,
	Health
};

class WaveSpawner
{
	// Members ************************************************************************************
public:

	unsigned int waveNumber;
	unsigned int waveEnemyCount;
	unsigned int waveEnemyCountCurrent;

	float enemySpawnTime;

	TArray<TPair<EnemyType, float>> spawnRates;
	float totalChance;

	ASphereWorldGameState* gameState;

private:
	float m_waveEnemyTrueCount;

	unsigned int enemiesSpawned;

	// Methods ************************************************************************************
public:

	void Init(ASphereWorldGameState* gameState);
	void Reset();

	void NewWave();

	void WaveStart();
	void WaveTick();

	EnemyType SpawnEnemy();

	void Swap(TPair<EnemyType, float>* xp, TPair<EnemyType, float>* yp);

	// A function to implement bubble sort
	void BubbleSort(TArray<TPair<EnemyType, float>> arr, int n);

};

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API ASphereWorldGameState : public AGameStateBase
{
	GENERATED_BODY()

	// Members ************************************************************************************
public:

	FTimerHandle m_spawningTicker;

protected:
	ASphereWorld* m_sphereWorld;

	TArray<AOrbitObject*> m_enemies;

	ARGameStates m_gameState;

	ACustomARPawn* m_pawn;

	AMainMenuHud* m_hud = nullptr;

	unsigned int m_score;

	WaveSpawner m_waveSpawner;

	// Methods ************************************************************************************
public:
	ASphereWorldGameState();

	inline ASphereWorld* GetSphereWorld() const { return m_sphereWorld; }
	inline void SetSphereWorld(ASphereWorld* newSphereWorld) { m_sphereWorld = newSphereWorld; }

	ASphereWorld* CreateSphereWorld(FVector worldPosition, FTransform trans);

	void SpawnEnemy();
	void SpawnControllerEnemy();

	inline ARGameStates GetGameState() const { return m_gameState;}
	void SetGameState(const ARGameStates& state);

	void SetPawn(ACustomARPawn* pawn);
	ACustomARPawn* GetPawn();

	AMainMenuHud* GetHUD();

	void CleanupSphereWorld();
	void CleanupEnemies();
	void CleanupGame();

	virtual void AddScore(const unsigned int& in_pts);
	virtual void SetScore(const unsigned int& in_pts);

	inline virtual unsigned int GetScore() const { return m_score; }

	static ASphereWorldGameState* Get(AActor* actor);
	static ASphereWorldGameState* Get(class UUserWidget* actor);

	void SpawnNewEnemy();
	void RemoveEnemy(AOrbitObject* oObject);
	void NextWave();

protected:

	void BeginGame();

	void OnPlayerDeath();

	void MainMenu();

	void SettingsMenu();
};


