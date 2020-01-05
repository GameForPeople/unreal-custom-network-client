// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// C++
#include <thread>
#include <atomic>

// Unreal
#include "Networking/Public/Networking.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "../../protocol.h"

#include "NetworkableGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UNetworkableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void StartNetwork(bool isUsePublicIP);

	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void EndNetwork();

	void NetworkFunction();

	void ProcessPacket();
	
	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void CallableTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void RegisterActor(int32 key, AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "WONSY_NETWORK")
	void ExcludeActor(int32 key);

	// UFUNCTION(BlueprintImplementableEvent, Category = "WONSY_NETWORK")
	// void AddObject(_KeyType key, _Int posX, _Int posY, _Byte type);
	// 
	// UFUNCTION(BlueprintImplementableEvent, Category = "WONSY_NETWORK")
	// void MoveObject(_KeyType key, _Int posX, _Int posY);
	// 
	// UFUNCTION(BlueprintImplementableEvent, Category = "WONSY_NETWORK")
	// void RemoveObject(_KeyType key);

private:
	FSocket* socket;

	uint8 recvBuffer[DEFINE::RECV_BUFFER_MAX_SIZE];
	uint8 loadedBuffer[DEFINE::PACKET_MAX_SIZE];
	uint8 loadedSize = 0;

	std::thread networkThread;
	std::atomic<bool> isNetworkEnd = true;

	TQueue<TArray<uint8>, EQueueMode::Mpsc> sendTaskQueue;
	TQueue<PACKET::DATA::BasePacket*, EQueueMode::Spsc> recvPacketQueue;
	TMap<uint32, AActor*> actorCont;
};
