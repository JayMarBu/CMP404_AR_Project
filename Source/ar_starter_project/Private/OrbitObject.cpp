// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObject.h"

// Sets default values
AOrbitObject::AOrbitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOrbitObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOrbitObject::Init(ASphereWorld* sphereWorld, FVector startPos)
{
	m_sphereWorld = sphereWorld;

	FacePlayer();
}

void AOrbitObject::FacePlayer()
{
	if (!m_sphereWorld || !m_sphereWorld->m_player)
		return;

	// face the player object
}

void AOrbitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

