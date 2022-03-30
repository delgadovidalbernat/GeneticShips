// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneticManager.generated.h"

class AEnemyManager;
class AChromosome;
class AEnemyActionPawn;
UCLASS()
class NAUSTFGMULTIPLAYER_API AGeneticManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneticManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<AChromosome*> GenerateNextGenerationDna(TArray<AChromosome*> actualGenerationDNA);

	TArray<AChromosome*> GenerateFirstGenerationDna();

	AChromosome* CrossOver(AChromosome* paren1, AChromosome* parent2);

	float CalculateRelativeAptitudes(TArray<float>& aptitudes, TArray<AChromosome*> population);
	
	TArray<AChromosome*> GetBestIndividues(TArray<AChromosome*> population, int32 numIndividues);

	int GetIndexChromosomeRulete(TArray<float> population, float rulete);

private:

	float CalculateAptitude(AChromosome* individual);

};






