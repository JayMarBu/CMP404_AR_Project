// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Camera/CameraComponent.h"		// Needs this to access Camera Component Functionality
#include"ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CustomActor.h"
#include "DrawDebugHelpers.h"
#include "SphereWorldGameState.h"
#include "SphereWorld.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
//#include "Kismet/KismetMathLibrary.h"


#define PRINT_DEBUG_LINE_TIME(_line_, _colour_, _time_) UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT(_line_)), true, true, _colour_, _time_)
#define PRINT_DEBUG_LINE(_line_, _colour_) PRINT_DEBUG_LINE_TIME(_line_, _colour_, 10)


// Sets default values
ACustomARPawn::ACustomARPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Scene COmponent as default
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	RootComponent = ScnComponent;

	//Setup Camera Component as default
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);


	ProjectileClass = AProjectile::StaticClass();

	// Timer Looping every 4 seconds
	cameraNotifyLoopTime = 4.0f;


}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::PrintString(this, FString(TEXT("Hello world")), true, true, FLinearColor(0, 0.66, 1, 1), 5);

	UARSessionConfig* Config = NewObject<UARSessionConfig>();
	//Config->
	UARBlueprintLibrary::StartARSession(Config);

#if PLATFORM_ANDROID
	UKismetSystemLibrary::PrintString(this, FString(TEXT("platform: Android")), true, true, FLinearColor(0, 0.66, 1, 1), 5);
#endif
#if PLATFORM_WINDOWS
	UKismetSystemLibrary::PrintString(this, FString(TEXT("platform: Windows")), true, true, FLinearColor(0, 0.66, 1, 1), 5);

#endif

	// Start my timer:
	//GetWorldTimerManager().SetTimer(cameraTicker, this, &ACustomARPawn::DisplayCameraInfo, cameraNotifyLoopTime, true, 0.0f);

	//  Spawn Custom Actor just once:
	//SpawnCube();
	SpawnSphereWorld();
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator camRot;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(m_viewLocation, camRot);

}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
}

// Print current camera orientation as debug message.
void ACustomARPawn::DisplayCameraInfo()
{
	// vars for camera location and rotation.
	FVector camLoc;
	FRotator camRot;

	// get camera details
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(camLoc, camRot);

	// Convert rotation into a vector as camera orientation
	FVector camOri = camRot.Vector();

	FVector pawnPos = this->GetActorLocation();
	FVector spherePos = m_sphereWorld->GetActorLocation();
	FVector playerPos = camLoc;

	// Print to screen
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Camera orientation: (%f, %f, %f)"), camOri.X, camOri.Y, camOri.Z), true, true, FLinearColor(0, 0.66, 1, 1), 5);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Camera orientation: (%f, %f, %f)"), camOri.X, camOri.Y, camOri.Z));
	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Blue, FString::Printf(TEXT("Pawn Position: [%f, %f, %f]\nsphere position [%f, %f, %f] \nplayer position [%f, %f, %f]"), 
		pawnPos.X, pawnPos.Y, pawnPos.Z,
		spherePos.X, spherePos.Y, spherePos.Z,
		playerPos.X, playerPos.Y, playerPos.Z
	));
}

void  ACustomARPawn::SpawnCube()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc(0, 0, 0);
	ACustomActor* customActor = GetWorld()->SpawnActor<ACustomActor>(this->GetActorLocation(), myRot, SpawnInfo);
}

void ACustomARPawn::SpawnSphereWorld()
{
	m_sphereWorld = GetWorld()->GetGameState<ASphereWorldGameState>()->CreateSphereWorld(FVector(0,0,0), this->GetActorTransform() );
	m_sphereWorld->m_player = this;
}

ASphereWorld* ACustomARPawn::GetSphereWorld()
{
	return nullptr;
}

FVector ACustomARPawn::GetViewLocation()
{
	return m_viewLocation;
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector worldPosition;
	FVector worldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, (FVector2D)screenPos, /*out*/worldPosition, /*out*/worldDirection);

	if (ProjectileClass)
	{
		FRotator camRot;
		FVector loc;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(loc, camRot);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, loc, camRot, SpawnParams);
		if (Projectile)
		{
			FVector LaunchDirection = worldDirection;//camRot.Vector();
			Projectile->FireInDirection(LaunchDirection, m_sphereWorld, ProjectileShooter::PLAYER);
		}
	}
}

void ACustomARPawn::OnActionTap()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("screen tapped")));
}

bool ACustomARPawn::WorlditTest(const FVector2D screenPos, FHitResult& /*out*/result)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/worldPosition, /*out*/worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceVector = worldDirection * 1000.0;
	traceVector = worldPosition + traceVector;
	// perform line trace
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(result, worldPosition, traceVector, ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}