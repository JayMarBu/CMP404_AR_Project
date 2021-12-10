// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class ASphereWorld;

enum class ProjectileShooter
{
	ENEMY,
	PLAYER
};

UCLASS()
class AR_STARTER_PROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	// Members ************************************************************************************
public:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;

		ASphereWorld* sphereWorld;

		ProjectileShooter projectileType;
	// Methods ************************************************************************************
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector velocity;

	virtual void InitialiseCollisionShape();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(FVector ShootDirection, ASphereWorld* in_sphereWorld, ProjectileShooter in_projectileType, const float& speed = 300.0f);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
