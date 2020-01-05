// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkableManagerActor.generated.h"

class UNetworkableGameInstance;

UCLASS()
class CLIENT_API ANetworkableManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkableManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void CallTickOfInstance(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void InitInstance();
private:
	UNetworkableGameInstance* networkableGameInstance;
};
