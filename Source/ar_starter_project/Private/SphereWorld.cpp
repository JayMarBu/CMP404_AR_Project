// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWorld.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASphereWorld::ASphereWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_scnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sphere world scene component"));
	RootComponent = m_scnComponent;

	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	
	m_staticMeshComponent->SetStaticMesh(MeshAsset.Object);
	m_staticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASphereWorld::BeginPlay()
{
	Super::BeginPlay();
	
	m_staticMeshComponent->SetRelativeScale3D(FVector(0.3,0.3,0.3));

	//SetActorScale3D(FVector(0.2,0.2,0.2));
//#ifdef PLATFORM_WINDOWS


	//FVector Loc = GetActorTransform().GetTranslation();

	//DrawDebugSphere(GetWorld(), Loc, m_spawnRadius, 26, FColor(181, 0, 0), true, 999, 0, 1);

//#endif // PLATFORM_WINDOWS
}

// Called every frame
void ASphereWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector Loc = GetActorTransform().GetTranslation();

	//DrawDebugSphere(GetWorld(), Loc, m_spawnRadius, 26, FColor(181, 0, 0), true, 1, 0, 1);
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

