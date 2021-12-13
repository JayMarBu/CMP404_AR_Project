
#include "GameplayGameMode.h"
#include "CustomARPawn.h"
#include "SphereWorldGameState.h"
//#include "UI/MainMenuHud.h"

AGameplayGameMode::AGameplayGameMode()
{
	DefaultPawnClass = ACustomARPawn::StaticClass();

	GameStateClass = ASphereWorldGameState::StaticClass();

}

void AGameplayGameMode::BeginGame()
{
}
