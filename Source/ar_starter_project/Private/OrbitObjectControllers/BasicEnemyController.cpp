// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/BasicEnemyController.h"
#include "OrbitObject.h"
#include "SphereWorld.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyProjectile.h"

UBasicEnemyController::UBasicEnemyController() 
{
	
}

void UBasicEnemyController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	m_timeCounter += DeltaTime;

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

	m_orbitObject->UpdatePosition();
	m_orbitObject->FacePlayer();
}

void UBasicEnemyController::TickMoveState(float DeltaTime)
{
	m_oscillationCounter += DeltaTime * m_oscillationSpeed;
	m_orbitObject->m_orbitTransform.OscillateAxis(FOrbitTransform::Y, m_baseLine, FMath::Sin(m_oscillationCounter) * (m_oscillationAmplitude));

	m_pingPongCounter += DeltaTime * m_pingPongSpeed;
	m_orbitObject->m_orbitTransform.OscillateAxis(FOrbitTransform::X, m_initXval, FMath::Cos(m_pingPongCounter) * m_pingPongAmplitude);

	if(m_state!= State::Attack && m_timeCounter > m_minShootTime)
	{
		float chance = FMath::RandRange(0, 10000);
		if(chance <= m_shootChance || m_timeCounter >= m_maxShootTime)
		{
			m_state = State::Attack;
			m_timeCounter = 0;
		}
	}
}

void UBasicEnemyController::TickShootState(float DeltaTime)
{
	m_timeCounter += DeltaTime;

	if(m_timeCounter > 0.5f)
	{
		Shoot();
	}

}

void UBasicEnemyController::TickDeadState(float DeltaTime)
{

}

void UBasicEnemyController::Shoot()
{
	FVector shootDir;

	shootDir = UKismetMathLibrary::GetDirectionUnitVector(m_orbitObject->GetActorLocation(),m_orbitObject->GetPlayerLocation());

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(m_orbitObject->GetActorLocation(),m_orbitObject->GetPlayerLocation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = m_orbitObject;
	SpawnParams.Instigator = m_orbitObject->GetInstigator();

	AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(AEnemyProjectile::StaticClass(), m_orbitObject->GetActorLocation()+(shootDir*55), rot, SpawnParams);
	if (Projectile)
	{
		FVector LaunchDirection = shootDir;//camRot.Vector();
		Projectile->FireInDirection(LaunchDirection, m_orbitObject->GetSphereWorld(), ProjectileShooter::ENEMY, 50);
	}

	m_state = State::Move;
	m_timeCounter = 0;
}

void UBasicEnemyController::OnHitCallback()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("You Hit Something")));
}

void UBasicEnemyController::Init(AOrbitObject* obj)
{
	Super::Init(obj);

	m_orbitObject->m_onProjectileHitDelegate.AddDynamic(this, &UBasicEnemyController::OnHitCallback);

	m_oscillationSpeed = FMath::RandRange(0.6f,1.2f);
	m_pingPongSpeed = FMath::RandRange(0.05f,0.2f);

	m_oscillationAmplitude = FMath::RandRange(10,20);
	m_pingPongAmplitude = FMath::RandRange(30,70);

	m_baseLine = m_orbitObject->m_orbitTransform.orbitPosition.Y;

	m_initXval = m_orbitObject->m_orbitTransform.orbitPosition.X;
}

AOrbitObject* UBasicEnemyController::SpawnBasicEnemy(AActor* actor, ASphereWorld* sWorld)
{
	if (actor == nullptr || sWorld == nullptr)
		return nullptr;

	float t = FMath::RandRange(0.0f, 360.0f);
	float s = FMath::RandRange(75.0f, 105.0f);

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = sWorld->GeneratePositionOnSphere(t, s, sWorld->m_spawnRadius);
	AOrbitObject* customActor = actor->GetWorld()->SpawnActor<AOrbitObject>(sWorld->m_player->GetActorLocation() + myLoc, myRot, SpawnInfo);

	customActor->Init(sWorld, FVector(t, s, sWorld->m_spawnRadius));

	customActor->AddControllerComponent<UBasicEnemyController>();

	return customActor;
}
