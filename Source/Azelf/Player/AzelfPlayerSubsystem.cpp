// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "AzelfPlayerSubsystem.h"

UAzelfPlayerSubsystem* UAzelfPlayerSubsystem::Get(const UObject& WorldContextObject)
{
    return WorldContextObject.GetWorld()->GetGameInstance()->GetSubsystem<ThisClass>();
}
