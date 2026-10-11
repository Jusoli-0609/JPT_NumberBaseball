// JPTGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JPTGameModeBase.generated.h"

class AJPTPlayerController;

/**
 *
 */
UCLASS()
class JPT_NUMBERBASEBALL_API AJPTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(AJPTPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(AJPTPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgeGame(AJPTPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AJPTPlayerController>> AllPlayerControllers;
};
