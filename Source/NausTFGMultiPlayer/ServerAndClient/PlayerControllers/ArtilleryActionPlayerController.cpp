// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtilleryActionPlayerController.h"

#include "ActionPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "NausTFGMultiPlayer/Client/Cameras/ActionCamera.h"
#include "NausTFGMultiPlayer/ServerAndClient/Pawns/ArtilleryActionPawn.h"
#include "NausTFGMultiPlayer/ServerAndClient/Pawns/PilotActionPawn.h"
#include "NausTFGMultiPlayer/Client/Controllers/ActionGameController.h"
#include "NausTFGMultiPlayer/ServerAndClient/PlayerStates/ActionPlayerState.h"

AArtilleryActionPlayerController::AArtilleryActionPlayerController()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Artillery Controller Prepared"));

	ConstructorHelpers::FClassFinder <AArtilleryActionPawn> refArtilleryActionPawnBP(TEXT("/Game/ServerAndClient/Pawns/ArtilleryActionPawn_BP"));
	reference = refArtilleryActionPawnBP.Class;

	cadencyOfTheGunInSeconds = 0.1f;
	bIsShooting = false;
	timeSinceLastProjectile = 0.f;
	lastTeamMatePosition = FVector::ZeroVector;
}

void AArtilleryActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Artillery Controller PreparedB"));

	CreaMenus();
	BindSignals();
	LoadHUD();
	
}

void AArtilleryActionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	timeSinceLastProjectile += DeltaSeconds;

	if(bIsShooting && timeSinceLastProjectile >= cadencyOfTheGunInSeconds)
	{

		timeSinceLastProjectile = 0.f;
		Fire();
	}
	
	if(teamMate)
	{
		APilotActionPawn* pilotMate = Cast<APilotActionPawn>(teamMate);

		AActionPlayerController* myPC = Cast<AActionPlayerController>(GetOwner());

		AArtilleryActionPawn* myPawn = nullptr;

		if(myPC)
			myPawn = Cast<AArtilleryActionPawn>(myPC->GetPawn());

		if (myPawn) {

			if ((myPawn->GetActorLocation() - pilotMate->GetActorLocation()).Size() > 2000.f)
			{

				myPawn->SetActorLocation(pilotMate->GetActorLocation());
			}

			if(pilotMate->GetActorLocation() != lastTeamMatePosition)
			{

				FVector nextLocationPawn = FMath::VInterpConstantTo(myPawn->GetActorLocation(), pilotMate->GetPredictedPosition(), DeltaSeconds, pilotMate->GetInterpolationSpeed());

				myPawn->SetActorLocation(nextLocationPawn);
			}else if(FMath::Abs((pilotMate->GetActorLocation() - lastTeamMatePosition).Size()) > 20.f)
			{
				FVector nextLocationPawn = FMath::VInterpConstantTo(myPawn->GetActorLocation(), pilotMate->GetActorLocation(), DeltaSeconds, pilotMate->GetMaxSpeed()/2);
				myPawn->SetActorLocation(nextLocationPawn);
			}

			lastTeamMatePosition = pilotMate->GetActorLocation();

			FRotator newRotation = myPawn->GetActorRotation();

			newRotation += FRotator(TestSensibility * DeltaSeconds * cameraInput.Y, TestSensibility * DeltaSeconds * cameraInput.X, 0.f);

			newRotation.Pitch = FMath::Clamp(newRotation.Pitch, -30.f, 30.f);

			myPawn->ExecuteRotation(newRotation);
			
		}

		if(cameraManager && Cast<AActionCamera>(cameraManager->GetViewTarget()))
		{

			AActionCamera* camera = Cast<AActionCamera>(cameraManager->GetViewTarget());

			
			if((camera->GetActorLocation() - pilotMate->GetActorLocation()).Size() > 2000.f)
			{

				camera->SetActorLocation(pilotMate->GetActorLocation());
			}
			

			FVector nextLocation = FMath::VInterpConstantTo(camera->GetActorLocation(), pilotMate->GetPredictedPosition(), DeltaSeconds, pilotMate->GetInterpolationSpeed());

			camera->SetActorLocation(myPawn->GetActorLocation());
			camera->SetActorRotation(myPawn->GetActorRotation());
		}
		
	}
}

UClass* AArtilleryActionPlayerController::GetDefaultPawn()
{
	return reference;
}

AActionCamera* AArtilleryActionPlayerController::SpawnActionCamera()
{

	AActionPlayerController* PC = Cast<AActionPlayerController>(GetOwner());
	AArtilleryActionPawn* pawn = Cast<AArtilleryActionPawn>(PC->GetPawn());

	FVector spawnLocation = pawn->GetActorLocation();
	FRotator spawnRotation = pawn->GetActorRotation();

	AActionCamera* camera = GetWorld()->SpawnActor<AActionCamera>(spawnLocation, spawnRotation);

	return camera;
}

void AArtilleryActionPlayerController::PitchCamera(float value)
{

	cameraInput.Y = value;

}

void AArtilleryActionPlayerController::YawCamera(float value)
{
	
	cameraInput.X = value;
}

void AArtilleryActionPlayerController::Fire()
{
	
	AActionPlayerController* playerController = Cast<AActionPlayerController>(GetOwner());
	Cast<AActionPawn>(playerController->GetPawn())->Fire();
	
}

void AArtilleryActionPlayerController::StopShooting()
{

	bIsShooting = false;
	AActionPlayerController* playerController = Cast<AActionPlayerController>(GetOwner());
	Cast<AActionPawn>(playerController->GetPawn())->StopFire();
}

float AArtilleryActionPlayerController::GetCadency()
{

	return cadencyOfTheGunInSeconds;
}

float AArtilleryActionPlayerController::GetTimeSinceLastProjectile()
{

	return timeSinceLastProjectile;
}

void AArtilleryActionPlayerController::CreaMenus()
{

	bool bIsLocalPlayerContropller = Cast<AActionPlayerController>(GetOwner())->IsLocalPlayerController();

	if (bIsLocalPlayerContropller && presentationController)
	{
		presentationController->CreaMenus();

	}
}

void AArtilleryActionPlayerController::LoadHUD()
{
	bool bIsLocalPlayerContropller = Cast<AActionPlayerController>(GetOwner())->IsLocalPlayerController();
	if (bIsLocalPlayerContropller)
	{

		Cast<UActionGameController>(presentationController)->LoadHUD();

	}
}

void AArtilleryActionPlayerController::BindSignals()
{
	
}

void AArtilleryActionPlayerController::StartShooting()
{

	bIsShooting = true;
}
