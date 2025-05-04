// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "AzelfPlayerSelf.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAzelfSelfAvatarChangeDelegate, APawn*, OldAvatar, APawn*, NewAvatar);

UCLASS()
class AZELF_API AAzelfPlayerSelf : public APawn, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AAzelfPlayerSelf();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
    UPROPERTY(BlueprintAssignable, Category="Azelf")
    FAzelfSelfAvatarChangeDelegate OnAvatarChanged;

protected:
    UPROPERTY(BlueprintReadOnly, Category="Azelf", ReplicatedUsing=OnRep_Avatar)
    TObjectPtr<APawn> Avatar;

    UFUNCTION(BlueprintNativeEvent, Category="Azelf")
    void OnRep_Avatar(APawn* OldAvatar);

    UFUNCTION(BlueprintCallable, Category="Azelf", Server, Reliable)
    void SetAvatar(APawn* NewAvatar);

    UFUNCTION(BlueprintPure, Category="Azelf")
    APlayerController* GetSelfPlayerController() const;

public:
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Azelf", meta=(DeterminesOutputType=PawnClass))
    APawn* Deploy(UPARAM(meta=(MustImplement ="/Script/Azelf.AzelfAvatarInterface"))
        TSubclassOf<APawn> PawnClass);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Azelf")
    bool Control(APawn* TargetPawn);
};
