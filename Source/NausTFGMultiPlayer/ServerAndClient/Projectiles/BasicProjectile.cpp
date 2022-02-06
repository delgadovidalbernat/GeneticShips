// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NausTFGMultiPlayer/ServerAndClient/Pawns/ActionPawn.h"

// Sets default values
ABasicProjectile::ABasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	projectileSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComponent"));
	projectileSphereComponent->InitSphereRadius(37.5f);
	projectileSphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = projectileSphereComponent;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	projectileMesh->SetupAttachment(RootComponent);

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	projectileMovementComponent->SetUpdatedComponent(projectileSphereComponent);
	projectileMovementComponent->InitialSpeed = 1500.0f;
	projectileMovementComponent->MaxSpeed = 1500.0f;
	projectileMovementComponent->bRotationFollowsVelocity = true;
	projectileMovementComponent->ProjectileGravityScale = 0.0f;

	damage = 10.f;
	damageType = UDamageType::StaticClass();

	if (GetLocalRole() == ROLE_Authority)
	{
		//projectileSphereComponent->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnProjectileImpact);
		projectileSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABasicProjectile::OnProjectileImpact);
	}
	
}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicProjectile::Destroyed()
{
	Super::Destroyed();

}

// Called every frame
void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AActionPawn* myPawn = Cast<AActionPawn>(GetInstigator());
	AActionPawn* otherPawn = Cast<AActionPawn>(OtherActor);

	//En caso de ser del mismo equipo no hacemos nada
	if(myPawn != nullptr && otherPawn != nullptr && myPawn->GetPawnTeamId().IsSet() && otherPawn->GetPawnTeamId().IsSet() && myPawn->GetPawnTeamId().GetValue() == otherPawn->GetPawnTeamId().GetValue())
	{

		return;
	}



	if (OtherActor && OtherActor->GetInstigatorController() != GetOwner()->GetInstigatorController())
	{
		UGameplayStatics::ApplyPointDamage(OtherActor->GetInstigatorController(), damage, FVector::ZeroVector, FHitResult(), GetOwner()->GetInstigatorController(), this, damageType);

		Destroy();
	}

	
}
