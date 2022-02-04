// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionPawn.h"
#include "ArtilleryActionPawn.generated.h"


class ABasicProjectile;
/**
 * 
 */
UCLASS()
class NAUSTFGMULTIPLAYER_API AArtilleryActionPawn : public AActionPawn
{
	GENERATED_BODY()

public:

	AArtilleryActionPawn();

	virtual void Fire() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();

protected:

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ABasicProjectile> projectile;

	
};
