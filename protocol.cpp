#include "protocol.h"

namespace PACKET::DATA
{
	BasePacket::BasePacket(const _Byte size, const _Enum type) noexcept
		: size(size)
		, type(type)
	{
	}

	namespace SERVER_TO_CLIENT
	{
		AddObject::AddObject(_KeyType key, _Int posX, _Int posY, _Byte objectType) noexcept
			: BasePacket(sizeof(AddObject), (_Enum)PACKET::TYPE::SERVER_TO_CLIENT::ADD_OBJECT)
			, key(key)
			, posX(posX)
			, posY(posY)
			, objectType(objectType)
		{
		}

		MoveObject::MoveObject(_KeyType key, _Int posX, _Int posY) noexcept
			: BasePacket(sizeof(MoveObject), (_Enum)PACKET::TYPE::SERVER_TO_CLIENT::MOVE_OBJECT)
			, key(key)
			, posX(posX)
			, posY(posY)
		{
		}

		RemoveObject::RemoveObject(_KeyType key) noexcept
			: BasePacket(sizeof(RemoveObject), (_Enum)PACKET::TYPE::SERVER_TO_CLIENT::REMOVE_OBJECT)
			, key(key)
		{
		}
	}

	namespace CLIENT_TO_SERVER
	{
		DemandMove::DemandMove(_Byte directionType) noexcept
			: BasePacket(sizeof(DemandMove), (_Enum)PACKET::TYPE::CLIENT_TO_SERVER::DEMAND_MOVE)
			, directionType(directionType)
		{
		}
	}
}