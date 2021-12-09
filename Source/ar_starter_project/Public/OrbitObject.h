// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereWorld.h"
#include "Misc/App.h"
#include "OrbitObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHitDelegate);

USTRUCT(BlueprintType)
struct FOrbitTransform
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D orbitPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float orbitRadius;

	FOrbitTransform() 
	: orbitPosition(0,0), orbitRadius(1) {}

	FOrbitTransform(FVector2D orbitPos, float orbitRad) 
	: orbitPosition(orbitPos), orbitRadius(orbitRad){}

	FOrbitTransform(float t, float s, float r)
	: orbitPosition(t,s), orbitRadius(r) {}

	FOrbitTransform(FVector orbitTransform)
	: orbitPosition(orbitTransform.X,orbitTransform.Y), orbitRadius(orbitTransform.Z){}

	operator FVector() const {return FVector(orbitPosition.X, orbitPosition.Y, orbitRadius);}

	FOrbitTransform& operator= (const FVector& oTransform)
	{
		orbitPosition.X = oTransform.X;
		orbitPosition.Y = oTransform.Y;
		orbitRadius = oTransform.Z;

		return *this;
	}

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
};

class USphereComponent;
class UOrbitObjectControllerBase;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* m_scnComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_staticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* m_collisionComponent;

	UPROPERTY(VisibleAnywhere)
	UOrbitObjectControllerBase* m_controllerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FOrbitTransform m_orbitTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool hasControllerComponent = false;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHitDelegate m_onProjectileHitDelegate;

	//Methods *************************************************************************************
public:	
	AOrbitObject();

protected:
	virtual void BeginPlay() override;

public:	

	void Init(ASphereWorld* sphereWorld, FVector startPos);

	void FacePlayer();

	virtual void Tick(float DeltaTime) override;

	inline void MoveAroundSphere(FVector moveOffset) {MoveAroundSphere(FVector2D(moveOffset.X, moveOffset.Y), moveOffset.Z);}
	void MoveAroundSphere(FVector2D orbitAngles, float zoomLevel);

	inline void MoveOrbit(FVector2D orbitDir){MoveAroundSphere(orbitDir, 0);}

	inline void MoveZoom(float zoomLevel){MoveAroundSphere(FVector2D(0,0), zoomLevel);}

	void UpdatePosition();

	void BroadcastHit();

	template <class ComponentClass>
	void AddControllerComponent();

};

template <class ComponentClass>
void AOrbitObject::AddControllerComponent()
{
	FName componentName("Controller Component");

	m_controllerComponent = NewObject<ComponentClass>(this, ComponentClass::StaticClass(), componentName);
	m_controllerComponent->Init(this);
	m_controllerComponent->RegisterComponent();
}




