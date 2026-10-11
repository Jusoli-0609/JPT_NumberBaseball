// JPTPawn.cpp


#include "JPTPawn.h"
#include "JPT_NumberBaseball.h"


void AJPTPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = JPT_NumberBaseballFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("JPTPawn::BeginPlay() %s [%s]"), *JPT_NumberBaseballFunctionLibrary::GetNetModeString(this), *NetRoleString);
	JPT_NumberBaseballFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AJPTPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = JPT_NumberBaseballFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("JPTPawn::PossessedBy() %s [%s]"), *JPT_NumberBaseballFunctionLibrary::GetNetModeString(this), *NetRoleString);
	JPT_NumberBaseballFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
