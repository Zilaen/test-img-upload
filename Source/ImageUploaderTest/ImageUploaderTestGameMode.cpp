// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImageUploaderTestGameMode.h"
#include "ImageUploaderTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AImageUploaderTestGameMode::AImageUploaderTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
