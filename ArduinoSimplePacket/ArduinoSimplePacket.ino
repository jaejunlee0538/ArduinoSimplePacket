#include "SimpleSerializer\DeserializeManager.h"
#include "PacketCount.h"
#include "PacketOnOff.h"
#include "PacketMove.h"

PacketOnOff sender_msg_on_off;
PacketCount sender_msg_count;

DeserializeManager manager;
PacketMove receiver_msg_move;
PacketOnOff receiver_msg_on_off;
PacketCount receiver_msg_count;
uint32_t count = 0;

String packet_stream;

void fillPacketStream(){
	String tmp;
	if (count % 5 == 0) {
		sender_msg_on_off._on_off = !sender_msg_on_off._on_off;
		sender_msg_on_off.serialize(tmp);
		packet_stream += tmp;
		packet_stream += "\n";
	}
	sender_msg_count._count = count++;
	sender_msg_count.serialize(tmp);
	packet_stream += tmp;
	packet_stream += "\n";
}

void readPacketStream(){
	while (packet_stream.length()) {
		manager.deserialize(packet_stream.substring(0,5));
		packet_stream.remove(0, 5);
	}
}

void setup()
{
	Serial.begin(115200);
	SerializeDeserializeExample();
	SerializeManagerExample();

	//manager.onDeserializeFailure(&callback_function1);
	//manager.onDeserializeSuccess(&callback_function2);
	manager.registerMessage(&receiver_msg_move);
	manager.registerMessage(&receiver_msg_on_off);
	manager.registerMessage(&receiver_msg_count);

	receiver_msg_count._count = 0;
}

void loop()
{
	fillPacketStream();
	readPacketStream();
	delay(500);

	if(receiver_msg_count.isUpdated()){
		Serial.print("Count : "); Serial.println(receiver_msg_count._count);
		receiver_msg_count.clearUpdated();
	}

	if (receiver_msg_move.isUpdated()) {
		Serial.print("Move X : "); Serial.println(receiver_msg_move.xyz[0]);
		receiver_msg_move.clearUpdated();
	}

	if (receiver_msg_on_off.isUpdated()) {
		Serial.print("On/Off : "); Serial.println(receiver_msg_on_off._on_off);
		receiver_msg_on_off.clearUpdated();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void deserialized(Serializable* message, const String& packet){
	Serial.print(packet);
	Serial.print("  ==>  ");
	String reconstructed;
	message->serialize(reconstructed);
	Serial.print("Success(");
	Serial.print(reconstructed);
	Serial.println(")");
}

void deserializeFailed(const Serializable* message, const String& packet){
	Serial.print(packet);
	Serial.print("  ==>  ");

	Serial.print("Fail(");
	if (message) {
		Serial.print("Error code = ");
		Serial.print(message->getLastError());
	}else{
		Serial.print("Unregistered packet type or invalid packet");
	}
	Serial.println(")");
}

void SerializeManagerExample() {
	Serial.println("----------SerializeManagerExample------------");
	PacketMove _msg_move;
	PacketOnOff _msg_on_off;

	//Sender
	PacketMove move_msg;
	PacketOnOff on_off_msg;
	move_msg.xyz[0] = 1.111;
	move_msg.xyz[1] = 2.222;
	move_msg.xyz[2] = 3.333;
	on_off_msg._on_off = true;
	String packet_stream, tmp;
	packet_stream.reserve(1024);

	move_msg.serialize(tmp);
	packet_stream += tmp;
	packet_stream += "\n";
	packet_stream += "#Unknown,ABC";
	packet_stream += "\n";
	on_off_msg.serialize(tmp);
	packet_stream += tmp;
	packet_stream += "\n";
	packet_stream += "This line should be skipped.";
	packet_stream += "\n";
	on_off_msg.serialize(tmp);
	tmp[7] = 'P';//one character is pollutted.
	packet_stream += tmp;
	packet_stream += "\n";
	
	Serial.println("Original Packet Stream : ");
	Serial.println(packet_stream);

	//Receiver
	DeserializeManager manager;
	manager.onDeserializeFailure(&deserializeFailed);
	manager.onDeserializeSuccess(&deserialized);
	manager.registerMessage(&_msg_move);
	manager.registerMessage(&_msg_on_off);
	
	//manager.deserialize(packet_stream);
	while (packet_stream.length()) {
		manager.deserialize(packet_stream.substring(0, 5));
		packet_stream.remove(0, 5);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SerializeDeserializeExample(){
	Serial.println("----------SerializeDeserializeExample------------");
	PacketMove packet_sent;
	PacketMove packet_received;

	packet_sent.xyz[0] = 1.12;
	packet_sent.xyz[1] = 2.12;
	packet_sent.xyz[2] = 3.12;
	Serial.print("Original data : ");
	Serial.print(packet_sent.xyz[0], 3); Serial.print(" ");
	Serial.print(packet_sent.xyz[1], 3); Serial.print(" ");
	Serial.print(packet_sent.xyz[2], 3); Serial.println("");

	String packet;
	if (packet_sent.serialize(packet)) {
		Serial.println("Created packet : \n\r" + packet);

		if (packet_received.isValid(packet)) {
			if (packet_received.deserialize(packet)) {
				Serial.print("Received data : ");
				Serial.print(packet_received.xyz[0], 3); Serial.print(" ");
				Serial.print(packet_received.xyz[1], 3); Serial.print(" ");
				Serial.print(packet_received.xyz[2], 3); Serial.println("");
			}
			else {
				Serial.println("Deserialize failed");
			}
		}
		else {
			Serial.println("Received packet is not valid");
		}
	}
	else {
		Serial.println("Serialize failed");
	}
	Serial.print("Last Error Code : ");
	Serial.println(packet_received.getLastError());
}