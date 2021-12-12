
#include "GameplayGameMode.h"
#include "GameplayHUD.h"
#include "CustomARPawn.h"
#include "GameplayHUDPlayerController.h"
#include "SphereWorldGameState.h"
//#include "UI/MainMenuHud.h"

AGameplayGameMode::AGameplayGameMode()
{
	DefaultPawnClass = ACustomARPawn::StaticClass();

	PlayerControllerClass = AGameplayHUDPlayerController::StaticClass();
	//if(m_menuHUDClass)
	//	HUDClass = m_menuHUDClass;

	GameStateClass = ASphereWorldGameState::StaticClass();

}

void AGameplayGameMode::BeginGame()
{
}
