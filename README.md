# ArduinoSimplePacket
Simple Comma-Separated ASCII Packet Serialization/Deserialization Modules

## Creating user-defined packet serializer

How to implement your own packets?

 1. Sub-classing `Serializable`
 2. Defining sub-class's data fields
 3. implement `Serializable::serializeImpl` and `Serializable::deserializeImpl` 
    for loading and unloading data fields defined at 2.
 4. implement `Serializable::getTypeField` such that a unique string is returned for distinguish each packets.

``` c++
#include "Serializable.h"

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

```

## Serialize/Deserialize Example

```c++
	PacketMove packet_sent;
	packet_sent.xyz[0] = 1.12;
	packet_sent.xyz[1] = 2.12;
	packet_sent.xyz[2] = 3.12;
	String packet;
	if (packet_sent.serialize(packet)) {	//Serialize(Packetizing)
		//send packet to the receiver.
		PacketMove packet_received;
		packet_received.deserialize(packet); //Deserialize(Parsing)
	}
```

## Deserialize Manager
``` c++
PacketMove _msg_move;
PacketOnOff _msg_on_off;

void setup(){
	DeserializeManager manager;
	//manager.onDeserializeFailure(&callback_function);
	//manager.onDeserializeSuccess(&callback_function);
	manager.registerMessage(&_msg_move);
	manager.registerMessage(&_msg_on_off);
}

void loop(){
	manager.deserialize(Serial.readString());

	if(_msg_move.updated()){
		//... read
		_msg_move.clearUpdated();
	}

	if(_msg_on_off.updated()){
		//... read
		_msg_on_off.clearUpdated();
	}
}
```
