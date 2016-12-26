// BytesBuffer.h

#ifndef _BYTESBUFFER_h
#define _BYTESBUFFER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class BytesBuffer{
public:
	/*
	Use internally allocated memory for the buffer.
	*/
	BytesBuffer(const size_t& len);
	
	/*
	Use externally allocated memory for the buffer 
	*/
	BytesBuffer(uint8_t* buffer, const size_t& len);

	~BytesBuffer();

	void clear();
	
	/*
	Byte length of data which has been loaded, or to be unloaded.
	*/
	const size_t& length() const;

	/*
	
	*/
	const uint8_t* get() const;

	bool load(const uint8_t& val);
	bool load(const uint16_t& val);
	bool load(const uint32_t& val);
	bool load(const float& val);
	bool load(const double& val);
	bool load(const char* val, const size_t& len);

	bool unload(uint8_t& val);
	bool unload(uint16_t& val);
	bool unload(uint32_t& val);
	bool unload(float& val);
	bool unload(double& val);
	bool unload(char* val, const size_t& len);
protected:
	uint8_t * _buffer;
};

#endif

