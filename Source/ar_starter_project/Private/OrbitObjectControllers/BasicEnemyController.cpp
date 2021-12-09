// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/BasicEnemyController.h"
#include "OrbitObject.h"

void UBasicEnemyController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	m_orbitObject->m_orbitTransform.AddOrbitOffset(10,20);

	m_orbitObject->UpdatePosition();
	m_orbitObject->FacePlayer();
}

void UBasicEnemyController::OnHitCallback()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, FString::Printf(TEXT("You Hit Something")));
}

void UBasicEnemyController::Init(AOrbitObject* obj)
{
	Super::Init(obj);

	m_orbitObject->m_onProjectileHitDelegate.AddDynamic(this, &UBasicEnemyController::OnHitCallback);
}
