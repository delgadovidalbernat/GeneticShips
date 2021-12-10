// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtilleryActionPawn.h"

AArtilleryActionPawn::AArtilleryActionPawn()
{

	bReplicates = true;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Artillery Pawn Prepared"));
}
