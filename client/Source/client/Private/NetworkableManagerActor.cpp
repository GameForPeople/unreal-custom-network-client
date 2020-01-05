// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkableManagerActor.h"
#include "NetworkableGameInstance.h"

// Sets default values
ANetworkableManagerActor::ANetworkableManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANetworkableManagerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANetworkableManagerActor::InitInstance()
{
	networkableGameInstance = static_cast<UNetworkableGameInstance*>(GetGameInstance());
}

// Called every frame
void ANetworkableManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANetworkableManagerActor::CallTickOfInstance(float DeltaTime)
{
	networkableGameInstance->CallableTick(DeltaTime);
}




