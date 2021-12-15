// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h" 
#include "Projectile.h"
//#include "SphereWorld.h"
#include "FMODEvent.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;
class ASphereWorld;
class USphereComponent;
//class UFMODEvent;

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

	// Timer methods 
	FTimerHandle cameraTicker;
	float cameraNotifyLoopTime;

	// Sphere world reference
	ASphereWorld* m_sphereWorld;

	// Camera position container
	FVector m_viewLocation;

	// HP Methods
	UPROPERTY(EditDefaultsOnly)
	unsigned int m_maxHP = 10;
	UPROPERTY(EditAnywhere)
	unsigned int m_currentHP;

	// SFX
	UPROPERTY(EditAnywhere)
	UFMODEvent* ShootSoundEvent;

	UPROPERTY(EditAnywhere)
	UFMODEvent* DamageSoundEvent;

	UPROPERTY(EditAnywhere)
	UFMODEvent* DieSoundEvent;

	UPROPERTY(EditAnywhere)
	UFMODEvent* WaveCompleteSoundEvent;

	// Shooting variables
	float m_cooldownTime = 0.2f;
	float m_cooldowntimer = 0;

	bool m_canShoot = true;

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

	void OnDeath();

	void SetSphereWorld(ASphereWorld* sWorld) {m_sphereWorld = sWorld;}

	virtual inline unsigned int GetMaxHealth() const { return m_maxHP;} 
	virtual inline unsigned int GetCurrentHealth() const { return m_currentHP;} 

	virtual void SetCurrentHealth(const unsigned int& hp);

	virtual void PlayWaveCompleteSound();

};
