// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NausTFGMultiPlayer/ServerAndClient/DataObjects/NausTFGEnums.h"
#include "AIBaseController.generated.h"

class AEnemyManager;
class APilotActionPawn;



/**
 * 
 */
UCLASS()
class NAUSTFGMULTIPLAYER_API AAIBaseController : public AAIController
{
	GENERATED_BODY()

public:

	AAIBaseController();

	void BindSignals();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void Destroyed() override;


public:

	float ApplyDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser);

	void MoveForward(float movement);

	void MoveRight(float movement);

	void OnEnemyDead();

	virtual void OnPossess(APawn* InPawn) override;
	

private:

	APilotActionPawn* target;

	float health;

	float fireTimeTest;
	float cadencyFire;

	EnemyState enemyState;
	
};
