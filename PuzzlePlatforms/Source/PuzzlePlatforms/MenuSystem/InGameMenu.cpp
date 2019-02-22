// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!QuitButton) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
	
	if (!CancelButton) { return false; }
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	return true;
}

void UInGameMenu::QuitPressed()
{
	if (!MenuInterface) { return; }
	
	MenuInterface->LoadMainMenu();
}

void UInGameMenu::CancelPressed()
{
	Teardown();
}