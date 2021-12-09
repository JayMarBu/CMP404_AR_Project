// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// Methods ************************************************************************************
public:	
	// Sets default values for this component's properties
	UOrbitObjectControllerBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnHitCallback();

	virtual void Init(AOrbitObject* obj);
};
