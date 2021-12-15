// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "OrbitObject.h"

// Sets default values for this component's properties
UOrbitObjectControllerBase::UOrbitObjectControllerBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOrbitObjectControllerBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("I have a Brain!")));
}


// Called every frame
void UOrbitObjectControllerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOrbitObjectControllerBase::OnHitCallback()
{
	// Respond to being hit by projectile by player
}

void UOrbitObjectControllerBase::Init(AOrbitObject* obj, int HP)
{
	// Set Object Reference
	m_orbitObject = obj;
	m_orbitObject->hasControllerComponent = true;

	// Set HP
	m_maxHP = HP;
	m_currentHP = m_maxHP;
}

// Static spawn function
AOrbitObject* UOrbitObjectControllerBase::SpawnBaseController(AActor* actor, ASphereWorld* sWorld)
{
	if (actor == nullptr || sWorld == nullptr)
		return nullptr;
	
	// Generate sphere spawn position
	float t = FMath::RandRange(0.0f, 360.0f);
	float s = FMath::RandRange(45.0f, 135.0f);

	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = sWorld->GeneratePositionOnSphere(t, s, sWorld->m_spawnRadius);

	// Spawn OrbitObject Actor
	AOrbitObject* customActor = actor->GetWorld()->SpawnActor<AOrbitObject>(sWorld->m_player->GetActorLocation() + myLoc, myRot, SpawnInfo);
	customActor->Init(sWorld, FVector(t, s, sWorld->m_spawnRadius), 1);

	// Add Controller Component
	customActor->AddControllerComponent<UOrbitObjectControllerBase>();

	return customActor;
}

