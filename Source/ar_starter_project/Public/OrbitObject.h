// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereWorld.h"
#include "Misc/App.h"
#include "OrbitObjectControllers/OrbitObjectControllerBase.h"
#include "OrbitObjectControllers/BasicEnemyController.h"
#include "OrbitObject.generated.h"

// On hit by projectile delegate type
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHitDelegate);

// Orbit transform struct ***************************************************************************************************************************
// - struct to act as a 3D transform component
//	 in a spherical orbit setting
USTRUCT(BlueprintType)
struct FOrbitTransform
{
	GENERATED_BODY()

	enum Axis
	{
		X,
		Y,
		Z
	};

	// Longitude and latitude angles
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D orbitPosition;

	// distance from the sphere center
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float orbitRadius;

	// Constructors
	FOrbitTransform() 
	: orbitPosition(0,0), orbitRadius(1) {}

	FOrbitTransform(FVector2D orbitPos, float orbitRad) 
	: orbitPosition(orbitPos), orbitRadius(orbitRad){}

	FOrbitTransform(float t, float s, float r)
	: orbitPosition(t,s), orbitRadius(r) {}

	FOrbitTransform(FVector orbitTransform)
	: orbitPosition(orbitTransform.X,orbitTransform.Y), orbitRadius(orbitTransform.Z){}

	// Operator overloads
	operator FVector() const {return FVector(orbitPosition.X, orbitPosition.Y, orbitRadius);}

	FOrbitTransform& operator= (const FVector& oTransform)
	{
		orbitPosition.X = oTransform.X;
		orbitPosition.Y = oTransform.Y;
		orbitRadius = oTransform.Z;

		return *this;
	}

	// Change position based on offset
	void AddOrbitOffset(float x, float y) { AddOrbitOffset(FVector2D(x,y));}

	void AddOrbitOffset(FVector2D offset)
	{
		orbitPosition += offset * FApp::GetDeltaTime();
		while (orbitPosition.X >= 360)
			orbitPosition.X = orbitPosition.X - 360;
		while (orbitPosition.Y >= 360)
			orbitPosition.Y = orbitPosition.Y - 360;

		while (orbitPosition.X < 0)
			orbitPosition.X = 360 + orbitPosition.X;
		while (orbitPosition.Y < 0)
			orbitPosition.Y = 360 + orbitPosition.Y;
	}

	// Oscillate position on a single axis
	void OscillateAxis(const Axis& a, const float& baseline, const float& func)
	{
		switch (a)
		{
		case X:
			orbitPosition.X = baseline + func;
		break;
		
		case Y:
			orbitPosition.Y = baseline + func;
		break;
		
		case Z:
			orbitRadius = baseline + func;
		break;
		}
	}
};

class USphereComponent;

// Orbit Object Actor Class *************************************************************************************************************************
// - Generic Actor class representing object orbiting the sphere world
UCLASS()
class AR_STARTER_PROJECT_API AOrbitObject : public AActor
{
	GENERATED_BODY()
	
	//Members *************************************************************************************
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ASphereWorld* m_sphereWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector m_initLocation;

public:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* m_scnComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_staticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* m_collisionComponent;

	UPROPERTY(VisibleAnywhere)
	UOrbitObjectControllerBase* m_controllerComponent;

	// Orbit transform
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FOrbitTransform m_orbitTransform;

	// On Hit Delegate
	UPROPERTY(BlueprintAssignable)
	FOnProjectileHitDelegate m_onProjectileHitDelegate;

	// Misc data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool hasControllerComponent = false;

	unsigned int m_scoreWorth;

	// Material Info
	UMaterial* m_storedMaterial;
	UMaterialInstanceDynamic* m_dynamicMaterialInst;

	//Methods *************************************************************************************
public:	
	AOrbitObject();

protected:
	virtual void BeginPlay() override;

public:	
	void Init(ASphereWorld* sphereWorld, FVector startPos, int HP);

	virtual void Tick(float DeltaTime) override;

	// Position methods
	void FacePlayer();
	void UpdatePosition();

	inline void MoveAroundSphere(FVector moveOffset) { MoveAroundSphere(FVector2D(moveOffset.X, moveOffset.Y), moveOffset.Z); }
	void MoveAroundSphere(FVector2D orbitAngles, float zoomLevel);

	inline void MoveOrbit(FVector2D orbitDir) { MoveAroundSphere(orbitDir, 0); }
	inline void MoveZoom(float zoomLevel) { MoveAroundSphere(FVector2D(0, 0), zoomLevel); }

	// Helper methods
	void BroadcastHit();

	template <class ComponentClass>
	void AddControllerComponent(int HP = 1);

	FVector GetPlayerLocation();

	inline ASphereWorld* GetSphereWorld() const { return m_sphereWorld;}
};

// Generic templated helper function for adding controller components
template <class ComponentClass>
void AOrbitObject::AddControllerComponent(int HP)
{
	FName componentName("Controller Component");

	m_controllerComponent = NewObject<ComponentClass>(this, ComponentClass::StaticClass(), componentName);
	m_controllerComponent->Init(this, HP);
	m_controllerComponent->RegisterComponent();
}
