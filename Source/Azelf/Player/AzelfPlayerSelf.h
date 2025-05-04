// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "AzelfPlayerSelf.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAzelfPlayerSelfAvatarChangeDelegate, APawn*, OldAvatar, APawn*, NewAvatar)
;

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
    virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1, bool bForce = false) override;

public:
    UPROPERTY(BlueprintAssignable, Category="Azelf")
    FAzelfPlayerSelfAvatarChangeDelegate OnAvatarChanged;

protected:
    UPROPERTY(BlueprintReadOnly, Category="Azelf", ReplicatedUsing=OnRep_Avatar)
    TObjectPtr<APawn> Avatar;

    UFUNCTION(BlueprintNativeEvent, Category="Azelf")
    void OnRep_Avatar(APawn* OldAvatar);

    UFUNCTION(BlueprintCallable, Category="Azelf", Server, Reliable)
    void SetAvatar(APawn* NewAvatar);

    UFUNCTION(BlueprintPure, Category="Azelf")
    APlayerController* GetSelfPlayerController() const;

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerSetAvatarAsAutonomousProxy();

    UFUNCTION(Client, Reliable,BlueprintCallable)
    void ClientSetAvatarAsAutoProxy();
};
