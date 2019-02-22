// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize() 
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!HostButton) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!JoinButton) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!BackButton) { return false; }
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!JoinIPButton) { return false; }
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!QuitButton) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	


	return true;
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) { return; }

	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!MenuInterface) { return; }

	FString Address = IPAddressField->GetText().ToString();
	MenuInterface->Join(Address);
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher) { return; }
	if (!JoinMenu) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher) { return; }
	if (!MainMenu) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	auto World = GetWorld();
	if (!ensure(World != nullptr)) { return; }
	auto PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ConsoleCommand("quit");
}