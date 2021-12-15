// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbitObject.h"
#include "FMODEvent.h"
#include "FMODBus.h"
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
	Bulky,
	Tank,
	Health
};

// Wave Spawner Class *******************************************************************************************************************************
// - helper class to manage wave spawning
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
	void BubbleSort(TArray<TPair<EnemyType, float>> arr, int n);
};

// Sphere World Game State Class ********************************************************************************************************************
// - Class to act as a globally accessible gameplay manager object
UCLASS()
class AR_STARTER_PROJECT_API ASphereWorldGameState : public AGameStateBase
{
	GENERATED_BODY()

	// Members ************************************************************************************
public:
	FTimerHandle m_spawningTicker;

	// FMOD Bus data
	UPROPERTY(EditDefaultsOnly)
	UFMODBus* Bus;

	float m_volume = 1;
	bool m_isMuted = false;

protected:
	// Gameplay data
	ASphereWorld* m_sphereWorld;
	TArray<AOrbitObject*> m_enemies;
	ACustomARPawn* m_pawn;
	WaveSpawner m_waveSpawner;

	// Game state flag
	ARGameStates m_gameState;

	// UI Data
	AMainMenuHud* m_hud = nullptr;
	unsigned int m_score;

	// FMOD Events
	UFMODEvent* EnemyShootSoundEvent;
	UFMODEvent* EnemyDamageSoundEvent;
	UFMODEvent* EnemyDeathSoundEvent;
	UFMODEvent* EnemySpawnSoundEvent;
	UFMODEvent* HealSoundEvent;

	// Methods ************************************************************************************
public:
	ASphereWorldGameState();

	// Sphere World methods
	inline ASphereWorld* GetSphereWorld() const { return m_sphereWorld; }
	inline void SetSphereWorld(ASphereWorld* newSphereWorld) { m_sphereWorld = newSphereWorld; }
	ASphereWorld* CreateSphereWorld(FVector worldPosition, FTransform trans);

	// Getters
	inline ARGameStates GetGameState() const { return m_gameState; }
	ACustomARPawn* GetPawn();
	AMainMenuHud* GetHUD();
	inline virtual unsigned int GetScore() const { return m_score; }

	// Setters
	void SetGameState(const ARGameStates& state);
	void SetPawn(ACustomARPawn* pawn);

	// Debug Methods
	void SpawnEnemy();
	void SpawnControllerEnemy();

	// Cleanup Methods
	void CleanupSphereWorld();
	void CleanupEnemies();
	void CleanupGame();

	// UI Update methods
	virtual void AddScore(const unsigned int& in_pts);
	virtual void SetScore(const unsigned int& in_pts);

	virtual void AddHealth(const unsigned int& hp);
	virtual void SetHealth(const unsigned int& hp);

	// Static global getter methods
	static ASphereWorldGameState* Get(AActor* actor);
	static ASphereWorldGameState* Get(class UUserWidget* actor);

	// Wave control methods
	void SpawnNewEnemy();
	void RemoveEnemy(AOrbitObject* oObject);
	void NextWave();

	// Sound related methods
	void PlayEnemyShootSound(AActor* actor);
	void PlayEnemyDamageSound(AActor* actor);
	void PlayEnemyDeathSound(AActor* actor);
	void PlayHealSound(AActor* actor);

	void SetVolume(float vol);
	void SetMuteState(bool isMuted);

	float GetVolume();
	bool GetMuteState();

protected:

	void BeginGame();

	void OnPlayerDeath();

	void MainMenu();

	void SettingsMenu();
};


