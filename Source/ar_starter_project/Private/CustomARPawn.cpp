// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Camera/CameraComponent.h"		// Needs this to access Camera Component Functionality
#include"ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "UI/MainMenuHud.h"
#include "FMODBlueprintStatics.h"


#define PRINT_DEBUG_LINE_TIME(_line_, _colour_, _time_) UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT(_line_)), true, true, _colour_, _time_)
#define PRINT_DEBUG_LINE(_line_, _colour_) PRINT_DEBUG_LINE_TIME(_line_, _colour_, 10)


ACustomARPawn::ACustomARPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//Setup Scene Component as default
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	RootComponent = ScnComponent;

	//Setup Camera Component as default
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);

	// Setup Collision Component as default
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Player"));

		float radius = 15.0f;
		CollisionComponent->InitSphereRadius(radius);
		CollisionComponent->SetupAttachment(ScnComponent);
	}

	// Set Projectile type
	ProjectileClass = AProjectile::StaticClass();

	// Timer Looping every 4 seconds
	cameraNotifyLoopTime = 4.0f;
}

void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

	// Initialise AR Session on startup
	UARSessionConfig* Config = NewObject<UARSessionConfig>();
	UARBlueprintLibrary::StartARSession(Config);

	// Set game state pawn reference
	GetWorld()->GetGameState<ASphereWorldGameState>()->SetPawn(this);

	// Init FMOD events
	ShootSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/shoot"));
	DamageSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/hit"));
	DieSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/death"));
	WaveCompleteSoundEvent = UFMODBlueprintStatics::FindEventByName(FString("event:/Game_Sounds/mixkit-unlock-game-notification-253"));
}

void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetWorld()->GetGameState<ASphereWorldGameState>()->GetGameState() != ARGameStates::Gameplay)
		return;
	
	// Reset shot cooldown timer
	if(!m_canShoot)
	{
		m_cooldowntimer += DeltaTime;
		if (m_cooldowntimer >= m_cooldownTime)
		{
			m_canShoot = true;
		}
	}

	// Calculate view position
	FRotator camRot;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(m_viewLocation, camRot);
	CollisionComponent->SetWorldLocation(m_viewLocation);
}

void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
}

void ACustomARPawn::SpawnSphereWorld()
{
	m_sphereWorld = GetWorld()->GetGameState<ASphereWorldGameState>()->CreateSphereWorld(FVector(0,0,0), this->GetActorTransform() );
	m_sphereWorld->m_player = this;
}

FVector ACustomARPawn::GetViewLocation()
{
	return m_viewLocation;
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	if(!m_canShoot)
		return;

	// Calculate screen space shot vector
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector worldPosition;
	FVector worldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, (FVector2D)screenPos, /*out*/worldPosition, /*out*/worldDirection);

	if (ProjectileClass)
	{
		// Calculate projectile spawn position
		FRotator camRot;
		FVector loc;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(loc, camRot);

		// Set spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Create projectile Actor
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, loc, camRot, SpawnParams);
		if (Projectile)
		{
			// Init projectile
			Projectile->FireInDirection(worldDirection, m_sphereWorld, ProjectileShooter::PLAYER);

			UFMODBlueprintStatics::PlayEventAtLocation(this, ShootSoundEvent, GetTransform(), true);

			// Set cool down
			m_cooldowntimer = 0;
			m_canShoot = false;
		}
	}
}

void ACustomARPawn::Hit()
{
	m_currentHP--;

	if(m_currentHP <= 0)
	{
		OnDeath();
		return;
	}

	// Update UI
	ASphereWorldGameState::Get(this)->GetHUD()->SetCurrentHealth(m_currentHP);

	// Play sound
	UFMODBlueprintStatics::PlayEventAtLocation(this, DamageSoundEvent, GetTransform(), true);
}

void ACustomARPawn::InitGame()
{
	m_currentHP = m_maxHP;

	SpawnSphereWorld();
}

void ACustomARPawn::OnDeath()
{
	// Play sound
	UFMODBlueprintStatics::PlayEventAtLocation(this, DieSoundEvent, GetTransform(), true);

	// Change game state
	ASphereWorldGameState* gs = GetWorld()->GetGameState<ASphereWorldGameState>();
	gs->GetHUD()->SetCurrentHealth(0);
	gs->SetGameState(ARGameStates::Death_menu);
}

void ACustomARPawn::SetCurrentHealth(const unsigned int& hp)
{
	 m_currentHP = (hp > m_maxHP) ? m_maxHP : hp;
	 ASphereWorldGameState::Get(this)->GetHUD()->SetCurrentHealth(m_currentHP);
}

void ACustomARPawn::PlayWaveCompleteSound()
{
	UFMODBlueprintStatics::PlayEventAtLocation(this, WaveCompleteSoundEvent, GetTransform(), true);
}
