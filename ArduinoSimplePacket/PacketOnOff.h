// PacketOnOff.h

#ifndef _PACKETONOFF_h
#define _PACKETONOFF_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "SimpleSerializer/Serializable.h"
class PacketOnOff:public Serializable{
public:
	const char* getTypeField() const {
		return "ONOFF";
	}

	bool serializeImpl(SerializeHelper::PacketLoader& loader) {
		if (_on_off)
			loader.load("On");
		else
			loader.load("Off");
		return true;//This is required.
	}

	bool deserializeImpl(SerializeHelper::PacketUnloader& unloader) {
		String field;
		if(!unloader.unload(field)){
			return false;
		}
		if (field.compareTo("On") == 0)
			_on_off = true;
		else if (field.compareTo("Off") == 0)
			_on_off = false;
		else
			return false;//parsing failed.
		return true;
	}

	bool _on_off;
};

#endif

