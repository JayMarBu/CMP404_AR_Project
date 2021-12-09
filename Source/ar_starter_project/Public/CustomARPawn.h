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

UCLASS()
class AR_STARTER_PROJECT_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;


	virtual void DisplayCameraInfo();
	virtual void SpawnCube();

	virtual void SpawnSphereWorld();

	virtual ASphereWorld* GetSphereWorld();

	virtual FVector GetViewLocation();

	virtual void OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	virtual void OnActionTap();
	bool WorlditTest(const FVector2D screenPos, FHitResult& /*out*/result);

private:

	FTimerHandle cameraTicker;
	float cameraNotifyLoopTime;

	ASphereWorld* m_sphereWorld;

	FVector m_viewLocation;

};
