// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ARBlueprintLibrary.h"
#include "SphereWorld.generated.h"


UCLASS()
class AR_STARTER_PROJECT_API ASphereWorld : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASphereWorld();

	// Members ************************************************************************************
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* m_scnComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_staticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere World Information")
	float m_spawnRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere World Information")
	AActor* m_player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UARPin* m_ARPin;

	// Methods ************************************************************************************

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	enum class AngleType
	{
		DEG,
		RAD
	};

	inline FVector GeneratePositionOnSphere(FVector info, AngleType type = AngleType::DEG) { return GeneratePositionOnSphere(info.X, info.Y, info.Z, type); }
	FVector GeneratePositionOnSphere(float t, float s, float r, AngleType type = AngleType::DEG);
};
