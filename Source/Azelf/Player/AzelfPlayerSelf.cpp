// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "AzelfPlayerSelf.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values
AAzelfPlayerSelf::AAzelfPlayerSelf()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAzelfPlayerSelf::BeginPlay()
{
    Super::BeginPlay();
}

void AAzelfPlayerSelf::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AAzelfPlayerSelf::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
}

void AAzelfPlayerSelf::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    constexpr FDoRepLifetimeParams Params = {
        .Condition = COND_None,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true
    };
    DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Avatar, Params);
}

UAbilitySystemComponent* AAzelfPlayerSelf::GetAbilitySystemComponent() const
{
    return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPlayerState());
}

void AAzelfPlayerSelf::AddMovementInput(const FVector WorldDirection, float ScaleValue, const bool bForce)
{
    Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
    Avatar->AddMovementInput(WorldDirection, ScaleValue, bForce);
}


void AAzelfPlayerSelf::SetAvatar_Implementation(APawn* NewAvatar)
{
    const auto OldAvatar = Avatar;
    if (OldAvatar != NewAvatar)
    {
        COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, Avatar, NewAvatar, this);
        OnRep_Avatar(OldAvatar);
    }
}

void AAzelfPlayerSelf::OnRep_Avatar_Implementation(APawn* OldAvatar)
{
    if (HasAuthority() == false)
    {
        Avatar->SetRole(ROLE_AutonomousProxy);
    }
    OnAvatarChanged.Broadcast(OldAvatar, Avatar);
}

APlayerController* AAzelfPlayerSelf::GetSelfPlayerController() const
{
    if (GetController())
    {
        return CastChecked<APlayerController>(GetController());
    }
    return nullptr;
}

void AAzelfPlayerSelf::ServerSetAvatarAsAutonomousProxy_Implementation()
{
    ensure(Avatar);
    Avatar->SetAutonomousProxy(true);
    ClientSetAvatarAsAutoProxy();
}

void AAzelfPlayerSelf::ClientSetAvatarAsAutoProxy_Implementation()
{
    // Avatar->bExchangedRoles = false;
    // Avatar->ExchangeNetRoles(true);
}
