// PacketMove.h

#ifndef _PACKETMOVE_h
#define _PACKETMOVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "SimpleSerializer/Serializable.h"

class PacketMove :public Serializable{
public:
	const char* getTypeField() const{
		return "MOVE";
	}

	bool serializeImpl(SerializeHelper::PacketLoader& loader){
		loader.load(String(xyz[0], 3));
		loader.load(String(xyz[1], 3));
		loader.load(String(xyz[2], 3));
		return true;//This is required.
	}

	bool deserializeImpl(SerializeHelper::PacketUnloader& unloader){
		String field;
		if (!unloader.unload(field)){ return false; }
		xyz[0] = field.toFloat();
		if (!unloader.unload(field)){ return false; }
		xyz[1] = field.toFloat();
		if (!unloader.unload(field)){ return false; }
		xyz[2] = field.toFloat();
		return true;
	}
	
	double xyz[3];
};

#endif

