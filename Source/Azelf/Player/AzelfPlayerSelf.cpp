// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "AzelfPlayerSelf.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AzelfAvatarInterface.h"
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

void AAzelfPlayerSelf::Tick(const float DeltaSeconds)
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

APawn* AAzelfPlayerSelf::Deploy(const TSubclassOf<APawn> PawnClass)
{
    ensure(PawnClass);
    ensure(PawnClass->ImplementsInterface(UAzelfAvatarInterface::StaticClass()));

    FActorSpawnParameters Params{};
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    Params.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
    Params.Instigator = this;
    FTransform SpawnTransform = GetTransform();
    SpawnTransform.SetScale3D(FVector(1.f));
    const auto NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, Params);
    return NewPawn;
}

bool AAzelfPlayerSelf::Control(APawn* TargetPawn)
{
    if (!TargetPawn || !TargetPawn->Implements<UAzelfAvatarInterface>())
    {
        checkNoEntry();
        return false;
    }

    SetAvatar(TargetPawn);
    const FAttachmentTransformRules Rule{
        EAttachmentRule::SnapToTarget,
        EAttachmentRule::SnapToTarget,
        EAttachmentRule::SnapToTarget,
        true
    };
    AttachToActor(TargetPawn, Rule);
    const auto PC = GetSelfPlayerController();
    PC->Possess(TargetPawn);
    return true;
}
