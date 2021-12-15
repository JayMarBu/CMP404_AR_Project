// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARBlueprintLibrary.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "OrbitObjectControllers/BasicEnemyController.h"
#include "GameplayGameMode.h"
#include "CustomARPawn.h"
#include "UI/MainMenuHud.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "OrbitObjectControllers/HealthObjectController.h"
#include "FMODBlueprintStatics.h"

ASphereWorldGameState::ASphereWorldGameState()
{
	m_gameState = ARGameStates::Main_Menu;

	m_waveSpawner.Init(this);

	EnemyShootSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/shoot"));
	EnemyDamageSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/enemyHit2"));
	HealSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/Heal"));
	EnemyDeathSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/enemyHit"));
	EnemySpawnSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/Enemy Spawn"));
}

ASphereWorld* ASphereWorldGameState::CreateSphereWorld(FVector worldPosition, FTransform trans)
{
	// Cleanup game before creating new sphere world
	CleanupSphereWorld();

	worldPosition = trans.GetTranslation();

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	m_sphereWorld = GetWorld()->SpawnActor<ASphereWorld>(worldPosition, myRot, SpawnInfo);

	return m_sphereWorld;
}

void ASphereWorldGameState::SpawnEnemy()
{
	if (!m_sphereWorld)
		return;

	float t = FMath::RandRange(0.0f, 360.0f);
	float s = FMath::RandRange(45.0f, 135.0f);

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = m_sphereWorld->GeneratePositionOnSphere(t, s, m_sphereWorld->m_spawnRadius);
	AOrbitObject* customActor = GetWorld()->SpawnActor<AOrbitObject>(m_sphereWorld->m_player->GetActorLocation() + myLoc, myRot, SpawnInfo);

	customActor->Init(m_sphereWorld, FVector(t, s, m_sphereWorld->m_spawnRadius), 1);

	m_enemies.Add(customActor);
}

void ASphereWorldGameState::SpawnControllerEnemy()
{
	if (!m_sphereWorld)
		return;

	m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this,m_sphereWorld));
}

void ASphereWorldGameState::SetGameState(const ARGameStates& state)
{
	m_gameState = state;

	// Set pawn and HUD objects if they aren't set
	if(!m_pawn)
		SetPawn(GetWorld()->GetFirstPlayerController()->GetPawn<ACustomARPawn>());
	if (!m_hud)
		m_hud = Cast<AMainMenuHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Run State transition methods
	switch (m_gameState)
	{
	case ARGameStates::Gameplay:
		BeginGame();
		break;

	case ARGameStates::Death_menu:
		OnPlayerDeath();
		break;

	case ARGameStates::Main_Menu:
		MainMenu();
		break;

	case ARGameStates::Options_Menu:
		SettingsMenu();
		break;
	}
}

void ASphereWorldGameState::BeginGame()
{
	// Cleanup previous game before starting new one
	CleanupGame();

	// destroy uneeded UI elements
	m_hud->HideMainMenu();
	m_hud->HideDeathScreen();
	m_hud->HideSettingsScreen();

	// create Gameplay UI objects
	m_hud->ShowGameHUD();

	// Init pawn
	m_pawn->InitGame();

	// Reset game data
	m_hud->SpawnHearts(m_pawn->GetMaxHealth());
	m_hud->SetCurrentHealth(m_pawn->GetMaxHealth());

	m_gameState = ARGameStates::Gameplay;
	SetScore(0);

	// Reset wave spawner
	m_waveSpawner.Reset();
	m_waveSpawner.WaveStart();
}

void ASphereWorldGameState::OnPlayerDeath()
{
	m_hud->HideMainMenu();
	m_hud->HideGameHUD();
	m_hud->HideDebugMenu();
	m_hud->HideSettingsScreen();

	m_hud->ShowDeathScreen();
	m_hud->SetScore(m_score);

	CleanupGame();
}

void ASphereWorldGameState::MainMenu()
{
	CleanupGame();

	m_hud->HideGameHUD();
	m_hud->HideDebugMenu();
	m_hud->HideDeathScreen();
	m_hud->HideSettingsScreen();

	m_hud->ShowMainMenu();

}

void ASphereWorldGameState::SettingsMenu()
{
	m_hud->HideGameHUD();
	m_hud->HideDebugMenu();
	m_hud->HideDeathScreen();
	m_hud->HideMainMenu();

	m_hud->ShowSettingsScreen();
}

void ASphereWorldGameState::SetPawn(ACustomARPawn* pawn)
{
	m_pawn = pawn;
}

ACustomARPawn* ASphereWorldGameState::GetPawn()
{
	return m_pawn;
}

AMainMenuHud* ASphereWorldGameState::GetHUD()
{
	return m_hud;
}

void ASphereWorldGameState::CleanupSphereWorld()
{
	if (!m_sphereWorld)
		return;

	m_sphereWorld->Destroy();
	m_sphereWorld = nullptr;
	
	// Make sure pawn always has valid reference to sphere world
	if(m_pawn)
		m_pawn->SetSphereWorld(nullptr);
}

void ASphereWorldGameState::CleanupEnemies()
{
	if(m_enemies.Num() <= 0)
		return;

	for (auto e: m_enemies)
	{
		if(e != nullptr)
		{
			e->Destroy();
			e = nullptr;
		}
	}

	m_enemies.Empty();
}

void ASphereWorldGameState::CleanupGame()
{
	CleanupSphereWorld();
	CleanupEnemies();
}

void ASphereWorldGameState::AddScore(const unsigned int& in_pts)
{
	m_score += in_pts;
	if(m_hud)
		m_hud->SetScore(m_score);
}

void ASphereWorldGameState::SetScore(const unsigned int& in_pts)
{
	m_score = in_pts;
	if (m_hud)
		m_hud->SetScore(m_score);
}

void ASphereWorldGameState::AddHealth(const unsigned int& hp)
{
	if(!m_pawn)
		return;

	m_pawn->SetCurrentHealth(m_pawn->GetCurrentHealth()+hp);
}

void ASphereWorldGameState::SetHealth(const unsigned int& hp)
{
	if (!m_pawn)
		return;

	m_pawn->SetCurrentHealth(hp);
}

ASphereWorldGameState* ASphereWorldGameState::Get(AActor* actor)
{
	return actor->GetWorld()->GetGameState<ASphereWorldGameState>();
}

ASphereWorldGameState* ASphereWorldGameState::Get(UUserWidget* widget)
{
	return widget->GetWorld()->GetGameState<ASphereWorldGameState>();
}

void ASphereWorldGameState::SpawnNewEnemy()
{
	// Calculate enemy type based on spawn weights
	EnemyType type = m_waveSpawner.SpawnEnemy();

	// Spawn relevant enemy type
	switch (type)
	{
	case EnemyType::Basic:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld, 1));
		break;
	case EnemyType::Bulky:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld, 2));
		break;
	case EnemyType::Tank:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld, 3));
		break;
	case EnemyType::Health:
		m_enemies.Add(UHealthObjectController::SpawnHealthObject(this, m_sphereWorld));
		break;
	}

	// Tick wave spawner
	m_waveSpawner.WaveTick();
}

void ASphereWorldGameState::RemoveEnemy(AOrbitObject* oObject)
{
	m_waveSpawner.waveEnemyCountCurrent--;
	m_hud->SetEnemyCount(m_waveSpawner.waveEnemyCountCurrent, m_waveSpawner.waveEnemyCount);
	m_enemies.Remove(oObject);
}

void ASphereWorldGameState::NextWave()
{	
	// start new wave if all enemies have been defeated
	if(m_waveSpawner.waveEnemyCountCurrent > 0)
		m_waveSpawner.WaveTick();
	else
	{
		m_pawn->PlayWaveCompleteSound();
		m_waveSpawner.WaveStart();
	}
}

void ASphereWorldGameState::PlayEnemyShootSound(AActor* actor)
{
	UFMODBlueprintStatics::PlayEventAtLocation(actor, EnemyShootSoundEvent, actor->GetTransform(), true);
}

void ASphereWorldGameState::PlayEnemyDamageSound(AActor* actor)
{
	UFMODBlueprintStatics::PlayEventAtLocation(actor, EnemyDamageSoundEvent, actor->GetTransform(), true);
}

void ASphereWorldGameState::PlayEnemyDeathSound(AActor* actor)
{
	UFMODBlueprintStatics::PlayEventAtLocation(actor, EnemyDeathSoundEvent, actor->GetTransform(), true);
}

void ASphereWorldGameState::PlayHealSound(AActor* actor)
{
	UFMODBlueprintStatics::PlayEventAtLocation(actor, HealSoundEvent, actor->GetTransform(), true);
}

void ASphereWorldGameState::SetVolume(float vol)
{
	m_volume = vol;
	UFMODBlueprintStatics::BusSetVolume(Bus, vol);
}

void ASphereWorldGameState::SetMuteState(bool isMuted)
{
	m_isMuted = isMuted;
	UFMODBlueprintStatics::BusSetMute(Bus, isMuted);
}

float ASphereWorldGameState::GetVolume()
{
	return m_volume;
}

bool ASphereWorldGameState::GetMuteState()
{
	return m_isMuted;
}

// **************************************************************************************************************************************************
// Wave Spawner Methods																																*
// **************************************************************************************************************************************************

void WaveSpawner::Init(ASphereWorldGameState* in_gameState)
{
	gameState = in_gameState;

	// Initialise spawn weights
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Basic, 65));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Bulky, 50));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Tank, 20));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Health, 25));

	// set total chance
	totalChance = 0;
	for (const TPair<EnemyType, float>& pair : spawnRates)
	{
		totalChance+= pair.Value;
	}

	// sort weights list from highest to lowest
	BubbleSort(spawnRates, spawnRates.Num());
}

void WaveSpawner::Reset()
{
	waveNumber = 0;
	waveEnemyCount = 6;
	m_waveEnemyTrueCount = 6;

	enemySpawnTime = 2;
}

void WaveSpawner::NewWave()
{
	// reset spawn counter
	enemiesSpawned = 0;

	//	increment wave counter
	waveNumber++;

	// calculate enemy count
	m_waveEnemyTrueCount = m_waveEnemyTrueCount + ((float)waveNumber*0.33f);
	waveEnemyCount = FMath::RoundToInt(m_waveEnemyTrueCount);
	waveEnemyCountCurrent = waveEnemyCount;

	// calculate spawn time
	enemySpawnTime = enemySpawnTime - (1/((float)waveEnemyCount*0.1f));

	// update HUD
	gameState->GetHUD()->SetEnemyCount(waveEnemyCountCurrent,waveEnemyCount);
	gameState->GetHUD()->SetWaveNum(waveNumber);
}

void WaveSpawner::WaveStart()
{
	NewWave();
	WaveTick();
}

void WaveSpawner::WaveTick()
{
	// set timer to spawn new enemy if enemies still need to be spawned
	if (enemiesSpawned >= waveEnemyCount)
	{
		// attempt to start next wave when all enemies are defeated
		gameState->GetWorldTimerManager().SetTimer(gameState->m_spawningTicker, gameState, &ASphereWorldGameState::NextWave, 2, false, 2);
	}
	else
	{
		gameState->GetWorldTimerManager().SetTimer(gameState->m_spawningTicker, gameState, &ASphereWorldGameState::SpawnNewEnemy, enemySpawnTime, false, enemySpawnTime);
	}

}

EnemyType WaveSpawner::SpawnEnemy()
{
	// increment spawn counter
	enemiesSpawned++;

	// Calculate enemy type based on spawn weights
	float r = FMath::RandRange(0.0f,totalChance);
	for (const TPair<EnemyType, float>& pair : spawnRates)
	{
		if(r <= pair.Value)
			return pair.Key;
		r-= pair.Value;
	}

	return spawnRates[spawnRates.Num()-1].Key;
}

void WaveSpawner::Swap(TPair<EnemyType, float>* xp, TPair<EnemyType, float>* yp)
{
	TPair<EnemyType, float> temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void WaveSpawner::BubbleSort(TArray<TPair<EnemyType, float>> arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j].Value < arr[j + 1].Value)
			{
				Swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}
