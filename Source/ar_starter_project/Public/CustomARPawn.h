// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h" 
#include "Projectile.h"
//#include "SphereWorld.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;
class ASphereWorld;
class USphereComponent;

UCLASS()
class AR_STARTER_PROJECT_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

	// Members ************************************************************************************
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* CollisionComponent;

protected:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle cameraTicker;
	float cameraNotifyLoopTime;

	ASphereWorld* m_sphereWorld;

	FVector m_viewLocation;

	UPROPERTY(EditDefaultsOnly)
	float m_maxHP = 10;

	UPROPERTY(EditAnywhere)
	float m_currentHP;

	// Methods ************************************************************************************
public:
	ACustomARPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SpawnSphereWorld();

	virtual FVector GetViewLocation();

	virtual void OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos);

	void Hit();

	void InitGame();

};
