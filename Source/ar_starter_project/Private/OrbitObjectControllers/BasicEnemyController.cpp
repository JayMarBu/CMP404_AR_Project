// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/BasicEnemyController.h"
#include "OrbitObject.h"
#include "SphereWorld.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyProjectile.h"
#include "SphereWorldGameState.h"

UBasicEnemyController::UBasicEnemyController() 
{

}

void UBasicEnemyController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Disable behavior if state is anything other than game play
	if(GetWorld()->GetGameState<ASphereWorldGameState>()->GetGameState() != ARGameStates::Gameplay)
		return;

	m_timeCounter += DeltaTime;

	// Kill entity
	if(m_currentHP <= 0)
	{
		ASphereWorldGameState::Get(m_orbitObject)->PlayEnemyDeathSound(m_orbitObject);
		m_state = State::Dead;
	}

	switch (m_state)
	{
	case UBasicEnemyController::State::Move:
		TickMoveState(DeltaTime);
		break;
	case UBasicEnemyController::State::Attack:
		TickShootState(DeltaTime);
		break;
	case UBasicEnemyController::State::Dead:
		TickDeadState(DeltaTime);
		break;
	}

	// update orbit object data
	m_orbitObject->UpdatePosition();
	m_orbitObject->FacePlayer();
}

void UBasicEnemyController::TickMoveState(float DeltaTime)
{
	// Move object in a randomized dual sin/cos wave motion
	m_oscillationCounter += DeltaTime * m_oscillationSpeed;
	m_orbitObject->m_orbitTransform.OscillateAxis(FOrbitTransform::Y, m_baseLine, FMath::Sin(m_oscillationCounter) * (m_oscillationAmplitude));

	m_pingPongCounter += DeltaTime * m_pingPongSpeed;
	m_orbitObject->m_orbitTransform.OscillateAxis(FOrbitTransform::X, m_initXval, FMath::Cos(m_pingPongCounter) * m_pingPongAmplitude);

	// Attack after randomized time
	if(m_state!= State::Attack && m_timeCounter > m_shootChance)
	{
		m_state = State::Attack;
		m_timeCounter = 0;
		m_shootChance = FMath::RandRange(m_minShootTime, m_maxShootTime);
	}
}

void UBasicEnemyController::TickShootState(float DeltaTime)
{
	m_timeCounter += DeltaTime;

	//TODO: Smooth the stopping motion

	//wait for a small amount of time before shooting
	if(m_timeCounter > 0.5f)
	{
		Shoot();
	}

}

void UBasicEnemyController::TickDeadState(float DeltaTime)
{
	ASphereWorldGameState::Get(m_orbitObject)->RemoveEnemy(m_orbitObject);
	m_orbitObject->Destroy();
}

void UBasicEnemyController::Shoot()
{
	// Calculate shooting vector and rotator
	FVector shootDir;
	shootDir = UKismetMathLibrary::GetDirectionUnitVector(m_orbitObject->GetActorLocation(),m_orbitObject->GetPlayerLocation());
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(m_orbitObject->GetActorLocation(),m_orbitObject->GetPlayerLocation());

	// Set spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = m_orbitObject;
	SpawnParams.Instigator = m_orbitObject->GetInstigator();

	// Spawn projectile Actor
	AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(AEnemyProjectile::StaticClass(), m_orbitObject->GetActorLocation()+(shootDir*55), rot, SpawnParams);
	if (Projectile)
	{
		// Set initial parameters
		Projectile->FireInDirection(shootDir, m_orbitObject->GetSphereWorld(), ProjectileShooter::ENEMY, 150);
		ASphereWorldGameState::Get(m_orbitObject)->PlayEnemyShootSound(m_orbitObject);
	}

	// reset state
	m_state = State::Move;
	m_timeCounter = 0;
}

void UBasicEnemyController::OnHitCallback()
{
	m_currentHP--;

	// Update texture to reflect loss of health
	m_orbitObject->m_dynamicMaterialInst->SetScalarParameterValue("HPCurrent", m_currentHP);

	ASphereWorldGameState::Get(m_orbitObject)->AddScore(m_orbitObject->m_scoreWorth);
	ASphereWorldGameState::Get(m_orbitObject)->PlayEnemyDamageSound(m_orbitObject);
}

void UBasicEnemyController::Init(AOrbitObject* obj, int HP)
{
	Super::Init(obj, HP);

	// Subscribe callback function for OrbitObject on hit
	m_orbitObject->m_onProjectileHitDelegate.AddDynamic(this, &UBasicEnemyController::OnHitCallback);

	// Randomize movement parameters
	m_oscillationSpeed = FMath::RandRange(0.6f,1.2f);
	m_pingPongSpeed = FMath::RandRange(0.05f,0.2f);

	m_oscillationAmplitude = FMath::RandRange(10,20);
	m_pingPongAmplitude = FMath::RandRange(30,70);

	// Set spawn location
	m_baseLine = m_orbitObject->m_orbitTransform.orbitPosition.Y;
	m_initXval = m_orbitObject->m_orbitTransform.orbitPosition.X;

	// Set score reward
	m_orbitObject->m_scoreWorth = 10*HP;
	
	// Initialise shoot chance
	m_shootChance = FMath::RandRange(m_minShootTime, m_maxShootTime);

	// Initialise texture information
	m_orbitObject->m_dynamicMaterialInst->SetScalarParameterValue("HPMax", (float)m_maxHP);
	m_orbitObject->m_dynamicMaterialInst->SetScalarParameterValue("HPCurrent", m_currentHP);

	m_orbitObject->m_dynamicMaterialInst->SetScalarParameterValue("isHealth", 0);
}

// Static spawn function
AOrbitObject* UBasicEnemyController::SpawnBasicEnemy(AActor* actor, ASphereWorld* sWorld, int HP)
{
	if (actor == nullptr || sWorld == nullptr)
		return nullptr;

	// Generate sphere spawn position
	float t = FMath::RandRange(0.0f, 360.0f);
	float s = FMath::RandRange(75.0f, 105.0f);

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = sWorld->GeneratePositionOnSphere(t, s, sWorld->m_spawnRadius);

	// Spawn OrbitObject Actor
	AOrbitObject* customActor = actor->GetWorld()->SpawnActor<AOrbitObject>(sWorld->m_player->GetActorLocation() + myLoc, myRot, SpawnInfo);
	customActor->Init(sWorld, FVector(t, s, sWorld->m_spawnRadius), HP);

	// Add Enemy Controller Component
	customActor->AddControllerComponent<UBasicEnemyController>(HP);

	return customActor;
}
