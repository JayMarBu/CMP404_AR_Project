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

ASphereWorldGameState::ASphereWorldGameState()
{
	m_gameState = ARGameStates::Main_Menu;

	m_waveSpawner.Init(this);
}

ASphereWorld* ASphereWorldGameState::CreateSphereWorld(FVector worldPosition, FTransform trans)
{
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

	if(!m_pawn)
		SetPawn(GetWorld()->GetFirstPlayerController()->GetPawn<ACustomARPawn>());

	if (!m_hud)
		m_hud = Cast<AMainMenuHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

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
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("Game Beginning...")));
	CleanupGame();

	m_hud->HideMainMenu();
	m_hud->HideDeathScreen();
	m_hud->HideSettingsScreen();

	m_hud->ShowDebugMenu();
	m_hud->ShowGameHUD();

	/*GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, UGameplayStatics::GetPlatformName());

	FARSessionStatus status = UARBlueprintLibrary::GetARSessionStatus();
	if (UGameplayStatics::GetPlatformName() == "PLATFORM_ANDROID" && status.Status != EARSessionStatus::Running)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Error starting AR session")));
		FString errorMessage = status.AdditionalInfo;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, errorMessage);
		MainMenu();
	}*/

	m_pawn->InitGame();

	m_hud->SpawnHearts(m_pawn->GetMaxHealth());
	m_hud->SetCurrentHealth(m_pawn->GetMaxHealth());

	m_gameState = ARGameStates::Gameplay;
	SetScore(0);

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
	
	if(m_pawn)
		m_pawn->SetSphereWorld(nullptr);
}

void ASphereWorldGameState::CleanupEnemies()
{
	if(m_enemies.Num() <= 0)
		return;

	for (auto e: m_enemies)
	{
		e->Destroy();
		e = nullptr;
	}

	m_enemies.Empty();
}

void ASphereWorldGameState::CleanupGame()
{
	CleanupSphereWorld();
	CleanupEnemies();

	//FARSessionStatus status = UARBlueprintLibrary::GetARSessionStatus();

	//if(status.Status == EARSessionStatus::Running)
	//	UARBlueprintLibrary::StopARSession();

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
	EnemyType type = m_waveSpawner.SpawnEnemy();

	switch (type)
	{
	case EnemyType::Basic:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld));
		break;
	case EnemyType::Bulky:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld, 2));
		break;
	case EnemyType::Tank:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld, 3));
		break;
	case EnemyType::Health:
		m_enemies.Add(UBasicEnemyController::SpawnBasicEnemy(this, m_sphereWorld));
		break;
	}

	m_waveSpawner.WaveTick();
}

void ASphereWorldGameState::RemoveEnemy(AOrbitObject* oObject)
{
	m_waveSpawner.waveEnemyCountCurrent--;
	m_hud->SetEnemyCount(m_waveSpawner.waveEnemyCountCurrent, m_waveSpawner.waveEnemyCount);
	m_enemies.Remove(oObject);
	SetScore(m_enemies.Num());
}

void ASphereWorldGameState::NextWave()
{

	if(m_waveSpawner.waveEnemyCountCurrent > 0)
		m_waveSpawner.WaveTick();
	else
		m_waveSpawner.WaveStart();
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("test")));
}


// **************************************************************************************************************************************************
// Wave Spawner Methods																																*
// **************************************************************************************************************************************************

void WaveSpawner::Init(ASphereWorldGameState* in_gameState)
{
	gameState = in_gameState;

	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Basic, 80));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Bulky, 50));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Tank, 10));
	spawnRates.Add(TPair<EnemyType, float>(EnemyType::Health, 20));

	totalChance = 0;
	for (const TPair<EnemyType, float>& pair : spawnRates)
	{
		totalChance+= pair.Value;
	}

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
	enemiesSpawned = 0;
	waveNumber++;

	m_waveEnemyTrueCount = m_waveEnemyTrueCount + ((float)waveNumber*0.33f);

	waveEnemyCount = FMath::RoundToInt(m_waveEnemyTrueCount);
	waveEnemyCountCurrent = waveEnemyCount;

	enemySpawnTime = enemySpawnTime - (1/((float)waveEnemyCount*0.33f));

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
	if (enemiesSpawned < waveEnemyCount)
	{
		gameState->GetWorldTimerManager().SetTimer(gameState->m_spawningTicker, gameState, &ASphereWorldGameState::SpawnNewEnemy, enemySpawnTime, false, enemySpawnTime);
		return;
	}

	gameState->GetWorldTimerManager().SetTimer(gameState->m_spawningTicker, gameState, &ASphereWorldGameState::NextWave, 2, false, 2);
}

EnemyType WaveSpawner::SpawnEnemy()
{
	enemiesSpawned++;

	float r = FMath::RandRange(0.0f,totalChance);

	for (const TPair<EnemyType, float>& pair : spawnRates)
	{
		if(r <= pair.Value)
		{
			return pair.Key;
		}

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
