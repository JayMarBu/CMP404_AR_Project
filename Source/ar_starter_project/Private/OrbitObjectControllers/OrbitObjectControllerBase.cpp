// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitObjectControllers/OrbitObjectControllerBase.h"

// Sets default values for this component's properties
UOrbitObjectControllerBase::UOrbitObjectControllerBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOrbitObjectControllerBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("I have a Brain!")));
}


// Called every frame
void UOrbitObjectControllerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOrbitObjectControllerBase::OnHitCallback()
{

}

void UOrbitObjectControllerBase::Init(AOrbitObject* obj)
{
	m_orbitObject = obj;
	m_orbitObject->hasControllerComponent = true;
}

