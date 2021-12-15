// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/Vector.h"
#include "SphereWorld.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include"Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "OrbitObject.h"
#include "SphereWorldGameState.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Root component
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Scene Component"));
	}


	// Setup Collision Component
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		// Initialise collision component in a subclass alterable way
		InitialiseCollisionShape();
		CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

		float radius = 15.0f;
		CollisionComponent->InitSphereRadius(radius);

		// Set collision component to root
		RootComponent = CollisionComponent;

		// Setup Mesh component
		if (!ProjectileMeshComponent)
		{
			ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
			static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
			if (Mesh.Succeeded())
			{
				ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
				ProjectileMeshComponent->SetupAttachment(RootComponent);
				
				// Calculate mesh scale based on collision shape size
				float fScale = (radius*2)/100;
				FVector vScale(fScale);

				ProjectileMeshComponent->SetRelativeScale3D(vScale);
			}

		}
	}

	// Setup Projectile movement component
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

	InitialLifeSpan = 5.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::InitialiseCollisionShape()
{
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetWorld()->GetGameState<ASphereWorldGameState>()->GetGameState() != ARGameStates::Gameplay)
		Destroy();

	ProjectileMovementComponent->Velocity = velocity;

}

void AProjectile::FireInDirection(FVector ShootDirection, ASphereWorld* in_sphereWorld, ProjectileShooter in_projectileType, const float& speed)
{
	switch (in_projectileType)
	{
	case ProjectileShooter::ENEMY:
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
		break;
	case ProjectileShooter::PLAYER:
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		break;
	}

	ShootDirection.Normalize();
	velocity = ShootDirection * speed;
	ProjectileMovementComponent->Velocity = velocity;

	sphereWorld = in_sphereWorld;
	projectileType = in_projectileType;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		// Collide with orbit objects only if this projectile was shot by the player
		UClass* HitActorClass = UGameplayStatics::GetObjectClass(Hit.GetActor());
		if (UKismetMathLibrary::ClassIsChildOf(HitActorClass, AOrbitObject::StaticClass()) && projectileType == ProjectileShooter::PLAYER)
		{
			AOrbitObject* actor = static_cast<AOrbitObject*>(OtherActor);
			
			actor->BroadcastHit();

			Destroy();
		}
		// Collide with the player only if this projectile was fired by an orbit object
		else if(UKismetMathLibrary::ClassIsChildOf(HitActorClass, ACustomARPawn::StaticClass()) && projectileType == ProjectileShooter::ENEMY)
		{
			ACustomARPawn* actor = static_cast<ACustomARPawn*>(OtherActor);

			actor->Hit();

			Destroy();
		}
		// Allow projectiles to destroy each other
		else if(UKismetMathLibrary::ClassIsChildOf(HitActorClass, AProjectile::StaticClass()))
		{
			AProjectile* actor = static_cast<AProjectile*>(OtherActor);

			if(projectileType != actor->projectileType)
			{
				OtherActor->Destroy();

				ASphereWorldGameState::Get(this)->AddScore(5);

				Destroy();
			}
		}
	}

}

