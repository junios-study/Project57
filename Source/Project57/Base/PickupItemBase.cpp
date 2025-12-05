// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Network/NetworkUtil.h"



// Sets default values
APickupItemBase::APickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(50.f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);
	bNetLoadOnClient = true;
	bNetUseOwnerRelevancy = true;
	
}

// Called when the game starts or when spawned
void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		NET_LOG(FString::Printf(TEXT("Weapon Owner %s"), *GetOwner()->GetName()));
	}
	else
	{
		NET_LOG(FString::Printf(TEXT("No Owner")));
	}
}

// Called every frame
void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

