// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerState.h"

#include "Net/UnrealNetwork.h"

AActionPlayerState::AActionPlayerState()
{

	maxHealth = 100.f;
	health = maxHealth;
}

void AActionPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActionPlayerState, health);
}

float AActionPlayerState::GetHealth()
{

	return health;
}

void AActionPlayerState::SetHealth(float value)
{

	health = value;
}

void AActionPlayerState::AddHealth(float value)
{

	if (health + value <= maxHealth)
		health += value;
}

void AActionPlayerState::OnHealthUpdate_Implementation()
{

	//lo que quiero que pase en todos los clientes cuando la vida se modifique
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Tu vida es: %f"), health));
}
