// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkableGameInstance.h"

void UNetworkableGameInstance::StartNetwork(bool isUsePublicIP)
{
	FString ipString = [&, isUsePublicIP]()noexcept -> FString
	{
		return isUsePublicIP ? DEFINE::PUBLIC_SERVER_IP : DEFINE::LOCAL_HOST_IP;
	}();

	TArray<uint8> ipCont;
	{
		TArray<FString> Tokens;
		if (ipString.ParseIntoArray(Tokens, TEXT("."), false) == 4)
		{
			for (int i = 0; i < 4; ++i)
			{
				//ipCont[i] = FCString::Atoi(*Tokens[i]); 
				ipCont.Add(FCString::Atoi(*Tokens[i]));
			}
		}
		else
		{
			UE_LOG(LogTemp, Fatal, TEXT("IP ���ۿ� �����߽��ϴ�."));
		}
	}

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetPort(DEFINE::MAIN_SERVER_PORT);
	addr->SetRawIp(ipCont);

	socket = FTcpSocketBuilder("ClientSocket")
		.AsReusable()
		.AsNonBlocking()
		.WithReceiveBufferSize(1000)
		.WithSendBufferSize(1000);

	if (socket->Connect(*addr))
	{
		UE_LOG(LogTemp, Display, TEXT("Ŀ��Ʈ�� �����߽��ϴ�."));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Ŀ��Ʈ�� �����߽��ϴ�."));
	}

	networkThread = std::thread([&]() { this->NetworkFunction(); });
	networkThread.detach();
}

void UNetworkableGameInstance::EndNetwork()
{
	isNetworkEnd = false;

	std::this_thread::sleep_for(std::chrono::seconds(5));

	if (socket)
	{
		socket->Close();
		delete socket;
	}
}

void UNetworkableGameInstance::NetworkFunction()
{
	while (isNetworkEnd)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));	// 1�ʿ� 10�� ��Ʈ��ũ

		// Send!
		TArray<uint8> sendedBuffer;
		while (sendTaskQueue.Dequeue(sendedBuffer))
		{
			int32 sendpacketSize{ sendedBuffer[0] };
			int32 sendedByteCount{ 0 };

			while (sendpacketSize != sendedByteCount)
			{
				int32 sendSuccessByteCount{ 0 };

				socket->Send(sendedBuffer.GetData() + sendedByteCount
					, sendedBuffer[0] - sendedByteCount
					, sendSuccessByteCount);

				sendedByteCount += sendSuccessByteCount;
			}
		}

		// Recv!
		uint32 recvSize;
		if (socket->HasPendingData(recvSize))
		{
			int32 recvSuccessByteCount;
			socket->Recv(recvBuffer, DEFINE::RECV_BUFFER_MAX_SIZE, recvSuccessByteCount, ESocketReceiveFlags::Type::None);

			uint8 packetSize;
			uint8* pBuf = recvBuffer;

			if (0 < loadedSize) packetSize = loadedBuffer[0];

			// ó������ ���� ������ ũ�Ⱑ ������, ��� �������� ���ϴ�.
			while (recvSuccessByteCount > 0)
			{
				// ������ ó���� ��ġ�� ���� ���۸� ó���ؾ��Ѵٸ� �н�, �ƴ϶�� ���� ó���ؾ��� ��Ŷ�� ũ�⸦ ����.
				if (packetSize == 0) packetSize = static_cast<uint8>(pBuf[0]);

				// ó���ؾ��ϴ� ��Ŷ ������ �߿���, ������ �̹� ó���� ��Ŷ ����� ���ش�.
				int required = packetSize - loadedSize;

				// ��Ŷ�� �ϼ��� �� ���� �� (��û�ؾ��� �������, ���� ����� ũ�ų� ���� ��)
				if (recvSuccessByteCount >= required)
				{
					memcpy(loadedBuffer + loadedSize, pBuf, required);

					//-------------------------------------------------------------------------------
					ProcessPacket();  // ��Ŷó��
					//-------------------------------------------------------------------------------

					loadedSize = 0;
					packetSize = 0;
					recvSuccessByteCount -= required;
					pBuf += required;
				}
				// ��Ŷ�� �ϼ��� �� ���� ��
				else
				{
					memcpy(loadedBuffer + loadedSize, pBuf, recvSuccessByteCount);
					loadedSize += recvSuccessByteCount;
					break;
				}
			}
		}
	}
}

void UNetworkableGameInstance::ProcessPacket()
{
	// using namespace PACKET::PACKET_TYPE;
	// switch (static_cast<MAIN_TO_CLIENT>(loadedBuffer[1]))
	// {
	// case MAIN_TO_CLIENT::MOVE:
	// 	break;
	// default:
	// 	UE_LOG(LogTemp, Display, TEXT("���ǵ��� ���� ��Ŷ�� �޾ҽ��ϴ�."));
	// 	break;
	// }
}


// use main thread
void UNetworkableGameInstance::CallableTick(float DeltaTime)
{
	using namespace PACKET::TYPE;
	using namespace PACKET::DATA::SERVER_TO_CLIENT;

	PACKET::DATA::BasePacket* retPacket;
	while (recvPacketQueue.Dequeue(retPacket))
	{
		switch (static_cast<SERVER_TO_CLIENT>(retPacket->type))
		{
		case SERVER_TO_CLIENT::ADD_OBJECT:
		{
			auto* packet = reinterpret_cast<PACKET::DATA::SERVER_TO_CLIENT::AddObject*>(&retPacket);
			//AddObject(packet->key, packet->posX, packet->posY, packet->objectType);


			delete packet;
		}
		break;
		case SERVER_TO_CLIENT::MOVE_OBJECT:
		{
			auto* packet = reinterpret_cast<PACKET::DATA::SERVER_TO_CLIENT::MoveObject*>(&retPacket);
			//MoveObject(packet->key, packet->posX, packet->posY);
			AActor** tempActor = actorCont.Find(packet->key);
			
			delete packet;
		}
		break;
		case SERVER_TO_CLIENT::REMOVE_OBJECT:
		{
			auto* packet = reinterpret_cast<PACKET::DATA::SERVER_TO_CLIENT::RemoveObject*>(&retPacket);
			//RemoveObject(packet->key);
			delete packet;
		}
		break;
		}
	}
}

void UNetworkableGameInstance::RegisterActor(int32 key, AActor* actor)
{
	actorCont.Add(key, actor);
}

void UNetworkableGameInstance::ExcludeActor(int32 key)
{
	actorCont.Remove(key);
}