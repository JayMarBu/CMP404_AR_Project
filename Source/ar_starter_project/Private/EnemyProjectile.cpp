// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/Vector.h"
#include "SphereWorld.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include"Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "OrbitObject.h"

void AEnemyProjectile::InitialiseCollisionShape()
{
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
}
