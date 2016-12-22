// PacketCount.h

#ifndef _PACKETCOUNT_h
#define _PACKETCOUNT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Serializable.h"
class PacketCount :public Serializable {
public:
	const char* getTypeField() const {
		return "COUNT";
	}

	bool serializeImpl(SerializeHelper::PacketLoader& loader) {
		loader.load(String(_count));
		return true;//This is required.
	}

	bool deserializeImpl(SerializeHelper::PacketUnloader& unloader) {
		String field;
		if (!unloader.unload(field)) {
			return false;
		}
		_count = field.toInt();
		return true;
	}

	uint32_t _count;
};

#endif

