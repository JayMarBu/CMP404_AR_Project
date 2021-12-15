// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "SphereWorldGameState.h"
#include "Components/SphereComponent.h"

AOrbitObject::AOrbitObject()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Scene Component as default
	m_scnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(m_scnComponent);

	// Setup Mesh Component
	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/HandheldARBP/CMP404_AR/enemies/untitled.untitled'"));
	m_staticMeshComponent->SetStaticMesh(MeshAsset.Object);
	m_staticMeshComponent->SetupAttachment(RootComponent);

	// Setup Material data
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("Material'/Game/HandheldARBP/CMP404_AR/enemies/EnemyMaterial.EnemyMaterial'"));
	if (FoundMaterial.Succeeded())
		m_storedMaterial = FoundMaterial.Object;
	m_dynamicMaterialInst = UMaterialInstanceDynamic::Create(m_storedMaterial, m_staticMeshComponent);
	m_staticMeshComponent->SetMaterial(0, m_dynamicMaterialInst);

	// Setup collision component
	if (!m_collisionComponent)
	{
		m_collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		m_collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("OrbitObject"));

		float radius = 35.0f;
		m_collisionComponent->InitSphereRadius(radius);

		m_collisionComponent->SetupAttachment(RootComponent);
	}
}

void AOrbitObject::BeginPlay()
{
	Super::BeginPlay();
}

void AOrbitObject::Init(ASphereWorld* sphereWorld, FVector startPos, int HP)
{
	m_sphereWorld = sphereWorld;

	m_initLocation = sphereWorld->m_player->GetActorLocation();

	m_orbitTransform = startPos;
	
	FacePlayer();
}

void AOrbitObject::FacePlayer()
{
	if (!m_sphereWorld )
		return;

	// face the player object
	FRotator lookatRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), m_sphereWorld->m_player->GetViewLocation());

	this->SetActorRotation(lookatRot);
}

void AOrbitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Allow controller component to update actor if it exists
	if(hasControllerComponent)
		return;

	UpdatePosition();
	FacePlayer();
}

void AOrbitObject::MoveAroundSphere(FVector2D orbitAngles, float zoomLevel)
{
	m_orbitTransform.AddOrbitOffset(orbitAngles);
	m_orbitTransform.orbitRadius = m_sphereWorld->m_spawnRadius + zoomLevel;
}

void AOrbitObject::UpdatePosition()
{
	if(!m_sphereWorld)
		return;

	FVector newLoc = m_sphereWorld->GetActorLocation() + m_sphereWorld->GeneratePositionOnSphere(m_orbitTransform);

	SetActorLocation(newLoc);
}

void AOrbitObject::BroadcastHit()
{
	m_onProjectileHitDelegate.Broadcast();
}

FVector AOrbitObject::GetPlayerLocation()
{
	return m_sphereWorld->m_player->GetViewLocation();
}

