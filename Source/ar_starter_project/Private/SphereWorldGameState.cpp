// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include "Math/UnrealMathUtility.h"
#include "ARBlueprintLibrary.h"

ASphereWorldGameState::ASphereWorldGameState()
{

}

ASphereWorld* ASphereWorldGameState::CreateSphereWorld(FVector worldPosition, FTransform trans)
{
	worldPosition = trans.GetTranslation();

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	m_sphereWorld = GetWorld()->SpawnActor<ASphereWorld>(worldPosition, myRot, SpawnInfo);

	m_sphereWorld->m_ARPin = UARBlueprintLibrary::PinComponent(m_sphereWorld->m_scnComponent, trans);

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
	AOrbitObject* customActor = GetWorld()->SpawnActor<AOrbitObject>(m_sphereWorld->GetActorLocation() + myLoc, myRot, SpawnInfo);

	UARBlueprintLibrary::PinComponentToARPin(customActor->m_scnComponent, m_sphereWorld->m_ARPin);

	customActor->Init(m_sphereWorld, FVector(t, s, m_sphereWorld->m_spawnRadius), m_sphereWorld->m_ARPin);

	m_enemies.Add(customActor);
}
