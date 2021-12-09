// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/Vector.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Scene Component"));
	}

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		float radius = 15.0f;

		CollisionComponent->InitSphereRadius(radius);

		RootComponent = CollisionComponent;

		if (!ProjectileMeshComponent)
		{
			ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
			static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
			if (Mesh.Succeeded())
			{
				ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
				ProjectileMeshComponent->SetupAttachment(RootComponent);
				
				float fScale = (radius*2)/100;
				FVector vScale(fScale);

				ProjectileMeshComponent->SetRelativeScale3D(vScale);
			}

		}
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 300.0f;
		ProjectileMovementComponent->MaxSpeed = 300.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::FireInDirection(FVector ShootDirection)
{
	ShootDirection.Normalize();
	ProjectileMovementComponent->Velocity = ShootDirection*ProjectileMovementComponent->InitialSpeed;
}

