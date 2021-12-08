// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Camera/CameraComponent.h"		// Needs this to access Camera Component Functionality
#include"ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CustomActor.h"
#include "DrawDebugHelpers.h"
#include "SphereWorldGameState.h"
#include "SphereWorld.h"
//#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
//#include "Runtime/Engine/Classes/Kismet/KiMath.h"

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
	GetWorldTimerManager().SetTimer(cameraTicker, this, &ACustomARPawn::DisplayCameraInfo, cameraNotifyLoopTime, true, 0.0f);

	//  Spawn Custom Actor just once:
	//SpawnCube();
	SpawnSphereWorld();
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

	// Print to screen
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Camera orientation: (%f, %f, %f)"), camOri.X, camOri.Y, camOri.Z), true, true, FLinearColor(0, 0.66, 1, 1), 5);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Camera orientation: (%f, %f, %f)"), camOri.X, camOri.Y, camOri.Z));
}

void  ACustomARPawn::SpawnCube()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc(150, 0, 0);
	ACustomActor* customActor = GetWorld()->SpawnActor<ACustomActor>(myLoc, myRot, SpawnInfo);
}

void ACustomARPawn::SpawnSphereWorld()
{
	m_sphereWorld = GetWorld()->GetGameState<ASphereWorldGameState>()->CreateSphereWorld(FVector(0,0,0), this->GetActorTransform() );
}

ASphereWorld* ACustomARPawn::GetSphereWorld()
{
	return nullptr;
}
