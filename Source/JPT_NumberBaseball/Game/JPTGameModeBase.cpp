// JPTGameModeBase.cpp


#include "Game/JPTGameModeBase.h"
#include "JPTGameStateBase.h"
#include "Player/JPTPlayerController.h"
#include "EngineUtils.h"
#include "Player/JPTPlayerState.h"

void AJPTGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	// AJPTGameStateBase* JPTGameStateBase =  GetGameState<AJPTGameStateBase>();
	// if (IsValid(JPTGameStateBase) == true)
	// {
	// 	JPTGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	// }
	//
	// AJPTPlayerController* JPTPlayerController = Cast<AJPTPlayerController>(NewPlayer);
	// if (IsValid(JPTPlayerController) == true)
	// {
	// 	AllPlayerControllers.Add(JPTPlayerController);
	// }

	AJPTPlayerController* JPTPlayerController = Cast<AJPTPlayerController>(NewPlayer);
	if (IsValid(JPTPlayerController) == true)
	{
		JPTPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(JPTPlayerController);

		AJPTPlayerState* JPTPS = JPTPlayerController->GetPlayerState<AJPTPlayerState>();
		if (IsValid(JPTPS) == true)
		{
			JPTPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AJPTGameStateBase* JPTGameStateBase = GetGameState<AJPTGameStateBase>();
		if (IsValid(JPTGameStateBase) == true)
		{
			JPTGameStateBase->MulticastRPCBroadcastLoginMessage(JPTPS->PlayerNameString);
		}
	}
}

FString AJPTGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AJPTGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString AJPTGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AJPTGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void AJPTGameModeBase::PrintChatMessageString(AJPTPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);

	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<AJPTPlayerController> It(GetWorld()); It; ++It)
		{
			AJPTPlayerController* JPTPlayerController = *It;
			if (IsValid(JPTPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				JPTPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<AJPTPlayerController> It(GetWorld()); It; ++It)
		{
			AJPTPlayerController* JPTPlayerController = *It;
			if (IsValid(JPTPlayerController) == true)
			{
				JPTPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void AJPTGameModeBase::IncreaseGuessCount(AJPTPlayerController* InChattingPlayerController)
{
	AJPTPlayerState* JPTPS = InChattingPlayerController->GetPlayerState<AJPTPlayerState>();
	if (IsValid(JPTPS) == true)
	{
		JPTPS->CurrentGuessCount++;
	}
}

void AJPTGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& JPTPlayerController : AllPlayerControllers)
	{
		AJPTPlayerState* JPTPS = JPTPlayerController->GetPlayerState<AJPTPlayerState>();
		if (IsValid(JPTPS) == true)
		{
			JPTPS->CurrentGuessCount = 0;
		}
	}
}

void AJPTGameModeBase::JudgeGame(AJPTPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AJPTPlayerState* JPTPS = InChattingPlayerController->GetPlayerState<AJPTPlayerState>();
		for (const auto& JPTPlayerController : AllPlayerControllers)
		{
			if (IsValid(JPTPS) == true)
			{
				FString CombinedMessageString = JPTPS->PlayerNameString + TEXT(" has won the game.");
				JPTPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& JPTPlayerController : AllPlayerControllers)
		{
			AJPTPlayerState* JPTPS = JPTPlayerController->GetPlayerState<AJPTPlayerState>();
			if (IsValid(JPTPS) == true)
			{
				if (JPTPS->CurrentGuessCount < JPTPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& JPTPlayerController : AllPlayerControllers)
			{
				JPTPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}