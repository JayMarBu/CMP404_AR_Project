// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SphereWorld.h"
#include "OrbitObjectControllerBase.generated.h"


class AOrbitObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AR_STARTER_PROJECT_API UOrbitObjectControllerBase : public UActorComponent
{
	GENERATED_BODY()

	// Members ************************************************************************************
public:
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AOrbitObject* m_orbitObject;

	UPROPERTY(EditAnywhere)
	float m_currentHP;

	UPROPERTY(EditAnywhere)
	float m_maxHP;

	// Methods ************************************************************************************
public:	
	UOrbitObjectControllerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnHitCallback();

	virtual void Init(AOrbitObject* obj, int HP);

	// Static Creation Function
	static AOrbitObject* SpawnBaseController(AActor* actor, ASphereWorld* sWorld);

};
