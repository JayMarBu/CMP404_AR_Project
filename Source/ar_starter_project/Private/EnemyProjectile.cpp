// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/Vector.h"
#include "SphereWorld.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include"Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "OrbitObject.h"

AEnemyProjectile::AEnemyProjectile()
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

		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

		float radius = 10.0f;
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

				float fScale = (radius * 2) / 100;
				FVector vScale(fScale);

				ProjectileMeshComponent->SetRelativeScale3D(vScale);
			}

		}
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 50.0f;
		ProjectileMovementComponent->MaxSpeed = 50.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	InitialLifeSpan = 5.0f;
}
