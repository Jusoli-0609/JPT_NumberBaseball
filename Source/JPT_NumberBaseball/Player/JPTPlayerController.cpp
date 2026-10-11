// JPTPlayerController.cpp

#include "Player/JPTPlayerController.h"

#include "UI/JPTChatInput.h"
#include <Kismet/KismetSystemLibrary.h>
#include "JPT_NumberBaseball.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/JPTGameModeBase.h"
#include "JPTPlayerState.h"
#include "Net/UnrealNetwork.h"

AJPTPlayerController::AJPTPlayerController()
{
	bReplicates = true;
}

void AJPTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UJPTChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AJPTPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		// ServerRPCPrintChatMessageString(InChatMessageString);

		AJPTPlayerState* JPTPS = GetPlayerState<AJPTPlayerState>();
		if (IsValid(JPTPS) == true)
		{
			//FString CombinedMessageString = JPTPS->PlayerNameString + TEXT(": ") + InChatMessageString;
			FString CombinedMessageString = JPTPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AJPTPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	// FString NetModeString = JPT_NumberBaseballFunctionLibrary::GetNetModeString(this);
	// FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	// JPT_NumberBaseballFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	JPT_NumberBaseballFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AJPTPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AJPTPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// for (TActorIterator<AJPTPlayerController> It(GetWorld()); It; ++It)
	// {
	// 	AJPTPlayerController* JPTPlayerController = *It;
	// 	if (IsValid(JPTPlayerController) == true)
	// 	{
	// 		JPTPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	// 	}
	// }

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AJPTGameModeBase* JPTGM = Cast<AJPTGameModeBase>(GM);
		if (IsValid(JPTGM) == true)
		{
			JPTGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
void AJPTPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}