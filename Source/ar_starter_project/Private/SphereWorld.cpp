// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorld.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ASphereWorld::ASphereWorld()
{
	PrimaryActorTick.bCanEverTick = true;

	m_scnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sphere world scene component"));
	RootComponent = m_scnComponent;
}

void ASphereWorld::BeginPlay()
{
	Super::BeginPlay();

}

void ASphereWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_player)
	{
		this->SetActorLocation(m_player->GetViewLocation());
	}
}

// Generates point on a sphere of radius [r] using latitude angle [t] and longitude angle [s]
FVector ASphereWorld::GeneratePositionOnSphere(
	float t,
	float s, 
	float r,
	AngleType type)
{
	if (type == AngleType::DEG)
	{
		t = FMath::DegreesToRadians(t);
		s = FMath::DegreesToRadians(s);
	}

	float x = r * FMath::Cos(t) * FMath::Sin(s);
	float y = r * FMath::Sin(t) * FMath::Sin(s);
	float z = r * FMath::Cos(s);

	return FVector(x, y, z);
}

