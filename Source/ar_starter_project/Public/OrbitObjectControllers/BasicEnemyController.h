// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "BasicEnemyController.generated.h"

class ASphereWorld;
/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UBasicEnemyController : public UOrbitObjectControllerBase
{
	GENERATED_BODY()

	// Members ************************************************************************************
protected:
	// General delta time counter
	float m_timeCounter = 0;
	FTimerHandle cameraTicker;

	// Shooting cool down variables
	float m_minShootTime = 4;
	float m_maxShootTime = 10;
	bool m_hasShotYet = false;
	float m_shootChance = 1;

	// Vertical Oscillation data
	float m_oscillationCounter = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_oscillationSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_oscillationAmplitude = 20;
	float m_baseLine = 90;

	// Horizontal Oscillation data
	float m_pingPongCounter = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_pingPongSpeed = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_pingPongAmplitude = 70;
	float m_initXval = 0;

public:
	
	enum class State
	{
		Move,
		Attack,
		Dead
	};

	State m_state = State::Move;
	
	// Methods ************************************************************************************
public:
	UBasicEnemyController();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickMoveState(float DeltaTime);
	void TickShootState(float DeltaTime);
	void TickDeadState(float DeltaTime);

	void Shoot();

	virtual void OnHitCallback() override;

	virtual void Init(AOrbitObject* obj, int HP);

	// Static Basic Enemy Creation function
	static AOrbitObject* SpawnBasicEnemy(AActor* actor, ASphereWorld* sWorld, int HP = 1);
};
