// JPTGameStateBase.cpp


#include "JPTGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/JPTPlayerController.h"

void AJPTGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AJPTPlayerController* JPTPC = Cast<AJPTPlayerController>(PC);
			if (IsValid(JPTPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				JPTPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
