// JPTGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JPTGameStateBase.generated.h"

/**
 *
 */
UCLASS()
class JPT_NUMBERBASEBALL_API AJPTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

};
