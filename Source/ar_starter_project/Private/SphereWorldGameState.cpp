// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include "Math/UnrealMathUtility.h"
#include "ARBlueprintLibrary.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "OrbitObjectControllers/BasicEnemyController.h"
#include "GameplayGameMode.h"
#include "CustomARPawn.h"
#include "UI/MainMenuHud.h"

ASphereWorldGameState::ASphereWorldGameState()
{
	m_gameState = ARGameStates::Main_Menu;
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

	customActor->Init(m_sphereWorld, FVector(t, s, m_sphereWorld->m_spawnRadius));

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

	switch (m_gameState)
	{
	case ARGameStates::Gameplay:
		BeginGame();
		break;

	case ARGameStates::Death_menu:
		OnPlayerDeath();
		break;

	case ARGameStates::Main_Menu:
		break;
	}
}

void ASphereWorldGameState::BeginGame()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("Game Beginning...")));
	CleanupGame();

	if(!m_hud)
		m_hud = Cast<AMainMenuHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	m_hud->HideMainMenu();
	m_hud->HideDeathScreen();

	m_hud->ShowDebugMenu();
	m_hud->ShowGameHUD();

	m_pawn->InitGame();

	m_hud->SpawnHearts(m_pawn->GetMaxHealth());
	m_hud->SetCurrentHealth(m_pawn->GetMaxHealth());

	m_gameState = ARGameStates::Gameplay;
	SetScore(0);
}

void ASphereWorldGameState::OnPlayerDeath()
{
	m_hud->HideMainMenu();
	m_hud->HideGameHUD();
	m_hud->HideDebugMenu();

	m_hud->ShowDeathScreen();
	m_hud->SetScore(m_score);

	CleanupGame();
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

ASphereWorldGameState* ASphereWorldGameState::Get(AActor* actor)
{
	return actor->GetWorld()->GetGameState<ASphereWorldGameState>();
}
