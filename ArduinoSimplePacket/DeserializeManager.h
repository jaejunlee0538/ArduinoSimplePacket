// SerializeManager.h

#ifndef _DESERIALIZEMANAGER_h
#define _DESERIALIZEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Serializable.h"

#define MAX_MESSAGES_NUM			30

typedef void(*onDeserializeSuccessCallback)(Serializable*, const String&);

/*
When appropriate Serializable instance was not found, NULL will be passed.
If it's not NULL, user can check error-code by calling getLastError().
*/
typedef void(*onDeserializeFailureCallback)(const Serializable*, const String&);

class DeserializeManager{

public:
	DeserializeManager(){
		//reserve (probably) enough buffer memory to prevent'fragmented heap'
		_buffer.reserve(512); 
	}

	void onDeserializeSuccess(onDeserializeSuccessCallback callback){
		_deserialize_success = callback;
	}

	void onDeserializeFailure(onDeserializeFailureCallback callback){
		_deserialize_fail = callback;
	}

	void deserialize(const String& data){
		int i_newline = data.indexOf("\n");
		if(i_newline < 0){
			_buffer += data;
			return;
		}
		_buffer += data.substring(0, i_newline);
		_buffer.trim();
		String type_field = Serializable::ExtractTypeField(_buffer);
		Serializable * msg = getMessage(type_field);
		if(msg == NULL || !msg->isValid(_buffer) || !msg->deserialize(_buffer)){
			if (_deserialize_fail)
				_deserialize_fail(msg, _buffer);
			_buffer = data.substring(i_newline + 1);
			return;
		}
		
		if (_deserialize_success)
			_deserialize_success(msg, _buffer);
		_buffer = data.substring(i_newline + 1);
	}

	bool registerMessage(Serializable* message){
		if (_count >= MAX_MESSAGES_NUM)
			return false;
		_message[_count++] = message;
	}
	
	Serializable* getMessage(const String& type_field){
		for(size_t i=0;i<_count;i++){
			if(type_field.compareTo(_message[i]->getTypeField()) == 0){
				return _message[i];
			}
		}
		return NULL;
	}
protected:
	size_t _count = 0;
	Serializable* _message[MAX_MESSAGES_NUM];
	onDeserializeSuccessCallback _deserialize_success = NULL;
	onDeserializeFailureCallback _deserialize_fail = NULL;
	String _buffer;
};

#endif

