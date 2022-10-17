/**
* File				: Queue.h (Queue.cpp)
* Class				: Queue
* Super Class		: 
* Function			:
* Latest Update		: 2017/08/10 - j.Noh
*/

#pragma once

constexpr auto BUFFER_SIZE = 4096;

class Queue
{
// Enums and structure ========================================================

// ============================================================================


// Variables ==================================================================
private:
	BYTE Buff[BUFFER_SIZE];

	int m_nTail;
	int m_nHead;

// ============================================================================


// Functions ==================================================================
public:
	Queue();
	~Queue();

	void Clear();
	int GetSize();
	bool PutByte(BYTE Byte);
	bool GetByte(BYTE* Byte);
	int PutByte(BYTE* ByteArray, int ByteSize);
	int GetByte(BYTE* ByteArray, int ByteSize);

// ============================================================================

};