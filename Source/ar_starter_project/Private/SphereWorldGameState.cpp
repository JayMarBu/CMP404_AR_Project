// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include "Math/UnrealMathUtility.h"
#include "ARBlueprintLibrary.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "OrbitObjectControllers/BasicEnemyController.h"
#include "GameplayGameMode.h"
#include "CustomARPawn.h"

ASphereWorldGameState::ASphereWorldGameState()
{
	m_gameState = ARGameStates::Main_Menu;
}

ASphereWorld* ASphereWorldGameState::CreateSphereWorld(FVector worldPosition, FTransform trans)
{
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

void ASphereWorldGameState::BeginGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("Game Beginning...")));

	AMainMenuHud* hud = Cast<AMainMenuHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	hud->HideMainMenu();
	hud->ShowDebugMenu();

	m_pawn->InitGame();
	m_pawn->SpawnSphereWorld();

	m_gameState = ARGameStates::Gameplay;
	m_score = 0;
}

void ASphereWorldGameState::SetPawn(ACustomARPawn* pawn)
{
	m_pawn = pawn;
}

ACustomARPawn* ASphereWorldGameState::GetPawn()
{
	return m_pawn;
}
