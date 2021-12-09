// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObject.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AOrbitObject::AOrbitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Scene COmponent as default
	m_scnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(m_scnComponent);

	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	// Get mesh from Unreal's Reference  Manager. (Right click on object and Get Reference"
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> MeshAsset(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));

	m_staticMeshComponent->SetStaticMesh(MeshAsset.Object);
	m_staticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOrbitObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(0.1f,0.5f,0.5f));
	
}

void AOrbitObject::Init(ASphereWorld* sphereWorld, FVector startPos)
{
	m_sphereWorld = sphereWorld;

	m_initLocation = sphereWorld->m_player->GetActorLocation();

	m_orbitTransform = startPos;

	FVector newLoc = m_sphereWorld->GetActorLocation() + m_sphereWorld->GeneratePositionOnSphere(m_initLocation);

	FacePlayer();
}

void AOrbitObject::FacePlayer()
{
	if (!m_sphereWorld )//|| !m_sphereWorld->m_player)
		return;

	// face the player object
	FRotator lookatRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), m_sphereWorld->m_player->GetActorLocation());

	this->SetActorRotation(lookatRot);
}

void AOrbitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Allow controller component to update actor
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

