// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereWorld.h"
#include "OrbitObject.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AOrbitObject : public AActor
{
	GENERATED_BODY()
	
	//Members *************************************************************************************
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ASphereWorld* m_sphereWorld;

	//Methods *************************************************************************************
public:	
	AOrbitObject();

protected:
	virtual void BeginPlay() override;

public:	

	void Init(ASphereWorld* sphereWorld, FVector startPos);

	void FacePlayer();

	virtual void Tick(float DeltaTime) override;

};
