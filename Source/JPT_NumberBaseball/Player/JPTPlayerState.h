// JPTPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JPTPlayerState.generated.h"

/**
 *
 */
UCLASS()
class JPT_NUMBERBASEBALL_API AJPTPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJPTPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
