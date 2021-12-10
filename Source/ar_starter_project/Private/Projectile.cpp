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

		InitialiseCollisionShape();
		CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

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

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::InitialiseCollisionShape()
{
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		UClass* HitActorClass = UGameplayStatics::GetObjectClass(Hit.GetActor());
		if (UKismetMathLibrary::ClassIsChildOf(HitActorClass, AOrbitObject::StaticClass()) && projectileType == ProjectileShooter::PLAYER)
		{
			AOrbitObject* actor = static_cast<AOrbitObject*>(OtherActor);
			
			actor->BroadcastHit();

			Destroy();
		}
		else if(UKismetMathLibrary::ClassIsChildOf(HitActorClass, ACustomARPawn::StaticClass()) && projectileType == ProjectileShooter::ENEMY)
		{
			ACustomARPawn* actor = static_cast<ACustomARPawn*>(OtherActor);

			actor->Hit();

			Destroy();
		}
		else if(UKismetMathLibrary::ClassIsChildOf(HitActorClass, AProjectile::StaticClass()))
		{
			AProjectile* actor = static_cast<AProjectile*>(OtherActor);

			if(projectileType != actor->projectileType)
			{
				OtherActor->Destroy();
				Destroy();
			}
		}
	}

}

