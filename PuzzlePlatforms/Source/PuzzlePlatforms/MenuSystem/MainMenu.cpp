// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/ServerRow_WBP"));
	if (!ServerRowBPClass.Class) { return; }

	ServerRowClass = ServerRowBPClass.Class;
	
}

bool UMainMenu::Initialize() 
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (HostButton == nullptr) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (JoinButton == nullptr) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (BackButton == nullptr) { return false; }
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (JoinIPButton == nullptr) { return false; }
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (QuitButton == nullptr) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (CancelHostButton == nullptr) { return false; }
	CancelHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (HostGameButton == nullptr) { return false; }
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface == nullptr) { return; }

	MenuInterface->Host(ServerName->GetText().ToString());
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (World == nullptr) { return; }

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames) {
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (Row != nullptr) {

			Row->ServerName->SetText(FText::FromString(ServerData.Name));
			Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
			Row->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
			Row->Setup(this, i);
			++i;

			ServerList->AddChild(Row);
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++) {
		UServerRow* ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (ServerRow != nullptr) {
				ServerRow->Selected = (SelectedIndex.IsSet() && i == SelectedIndex.GetValue());
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d."), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr) {
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher == nullptr) { return; }
	if (MainMenu == nullptr) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher == nullptr) { return; }
	if (HostMenu == nullptr) { return; }
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::QuitGame()
{
	auto World = GetWorld();
	if (!ensure(World != nullptr)) { return; }
	auto PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ConsoleCommand("quit");
}
