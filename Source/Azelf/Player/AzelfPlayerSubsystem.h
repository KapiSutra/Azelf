// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AzelfPlayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AZELF_API UAzelfPlayerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    static UAzelfPlayerSubsystem* Get(const UObject& WorldContextObject);
};
