#pragma once

#ifdef SERVER_PROTOCOL
#define UENUM(x) /* void */
#define UCLASS(X) /* void */
#define UMETA(x) /* void */
#else
#define CLIENT_PROTOCOL
#endif

namespace USING
{
#ifdef SERVER_PROTOCOL
	using _String = char[];
	using _Enum = unsigned char;

	using _Byte = unsigned char;
	using _Short = unsigned short;
	using _Int = unsigned int;
	using _Long = unsigned long long;
#else // unreal
	using _String = FString;
	using _Enum = uint8;

	using _Byte = uint8;
	using _Short = uint16;
	using _Int = uint32;
	using _Long = uint64;
#endif
	using _KeyType = _Int;
}using namespace USING;

namespace DEFINE
{
	const _String PUBLIC_SERVER_IP = "13.125.73.63";
	const _String LOCAL_HOST_IP = "127.0.0.1";

	constexpr static _Short MAIN_SERVER_PORT = 9000;
	constexpr static _Short RECV_BUFFER_MAX_SIZE = 1025;
	constexpr static _Short PACKET_MAX_SIZE = 256;
}

namespace PACKET
{
	namespace TYPE
	{
		UENUM(BlueprintType)
		enum class SERVER_TO_CLIENT : _Enum
		{
			ADD_OBJECT UMETA(DisplayName = "ADD_OBJECT"),
			MOVE_OBJECT UMETA(DisplayName = "MOVE_OBJECT"),
			REMOVE_OBJECT UMETA(DisplayName = "REMOVE_OBJECT")
		};

		UENUM(BlueprintType)
		enum class CLIENT_TO_SERVER : _Enum
		{
			DEMAND_MOVE UMETA(DisplayName = "DEMAND_MOVE")
		};
	}

#pragma pack(push, 1)
	namespace DATA
	{
		struct BasePacket
		{
			const _Byte size;
			const _Enum type;

			explicit BasePacket(const _Byte size, const _Enum type) noexcept;
		};

		namespace SERVER_TO_CLIENT
		{
			struct AddObject : public BasePacket
			{
				const _KeyType key;
				const _Int posX;
				const _Int posY;
				const _Byte objectType;

				explicit AddObject(_KeyType key, _Int posX, _Int posY, _Byte objectType) noexcept;
			};

			struct MoveObject : public BasePacket
			{
				const _KeyType key;
				const _Int posX;
				const _Int posY;

				explicit MoveObject(_KeyType key, _Int posX, _Int posY) noexcept;
			};

			struct RemoveObject : public BasePacket
			{
				const _KeyType key;

				explicit RemoveObject(_KeyType key) noexcept;
			};
		}

		namespace CLIENT_TO_SERVER
		{
			struct DemandMove : public BasePacket
			{
				const _Byte directionType; // 0 left, 1 right, 2 up, 3 down

				explicit DemandMove(_Byte directionType) noexcept;
			};
		}
	}
#pragma pack(pop)
}
