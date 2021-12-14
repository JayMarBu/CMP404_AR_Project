// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/HealthObjectController.h"
#include "SphereWorldGameState.h"

UHealthObjectController::UHealthObjectController()
{

}

void UHealthObjectController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (GetWorld()->GetGameState<ASphereWorldGameState>()->GetGameState() != ARGameStates::Gameplay)
		return;

	m_timeCounter += DeltaTime;

	if (m_currentHP <= 0)
		m_state = State::Dead;

	switch (m_state)
	{
	case UBasicEnemyController::State::Move:
	case UBasicEnemyController::State::Attack:
		TickMoveState(DeltaTime);
		break;
	case UBasicEnemyController::State::Dead:
		TickDeadState(DeltaTime);
		break;
	}

	m_orbitObject->UpdatePosition();
	m_orbitObject->FacePlayer();
}

void UHealthObjectController::Init(AOrbitObject* obj, int HP)
{
	UOrbitObjectControllerBase::Init(obj, HP);	

	m_orbitObject->m_onProjectileHitDelegate.AddDynamic(this, &UHealthObjectController::OnHitCallback);

	m_oscillationSpeed = FMath::RandRange(0.6f, 1.2f);
	m_pingPongSpeed = FMath::RandRange(0.1f, 0.3f);

	m_oscillationAmplitude = FMath::RandRange(10, 20);
	m_pingPongAmplitude = FMath::RandRange(45, 90);

	m_baseLine = m_orbitObject->m_orbitTransform.orbitPosition.Y;

	m_initXval = m_orbitObject->m_orbitTransform.orbitPosition.X;

	m_maxHP = HP;
	m_currentHP = m_maxHP;

	m_orbitObject->m_scoreWorth = 50;

	m_shootChance = FMath::RandRange(m_minShootTime, m_maxShootTime);
}

void UHealthObjectController::OnHitCallback()
{
	m_state = UBasicEnemyController::State::Dead;
	ASphereWorldGameState::Get(m_orbitObject)->AddHealth(5);
}
