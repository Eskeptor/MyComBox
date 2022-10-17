#include "stdafx.h"
#include "Queue.h"

Queue::Queue()
{
	Clear();
}

Queue::~Queue()
{
}

void Queue::Clear()
{
	m_nTail = m_nHead = 0;
	memset(Buff, 0, BUFFER_SIZE);
}

int Queue::GetSize()
{
	// 데이터가 순환하여 입력된 경우
	if (0 > (m_nTail - m_nHead) % BUFFER_SIZE)
		return (m_nTail + BUFFER_SIZE - m_nHead) % BUFFER_SIZE;

	// 데이터가 순환하지 않고 입력된 경우
	return (m_nTail - m_nHead) % BUFFER_SIZE;
}

bool Queue::PutByte(BYTE Byte)
{
	// 더이상 입력할 공간이 하나도 없을 때
	if ((BUFFER_SIZE -1) == GetSize())
		return false;

	// 1 Byte 입력
	Buff[m_nTail++] = Byte;

	// Tail을 순환
	m_nTail %= BUFFER_SIZE;

	return true;
}

bool Queue::GetByte(BYTE* Byte)
{
	// 빼올 데이터가 하나도 없을경우
	if (0 == GetSize())
		return false;

	// 1 Byte 출력
	*Byte = Buff[m_nHead++];

	// Head를 순환
	m_nHead %= BUFFER_SIZE;

	return true;
}

int Queue::PutByte(BYTE* ByteArray, int ByteSize)
{
	// size만큼 입력할 수 없을 때
	if (GetSize() + ByteSize > BUFFER_SIZE - 1)
		return FALSE;

	// 배열이 순환해야 하는 경우
	if (m_nTail + ByteSize > BUFFER_SIZE - 1)
	{
		// ByteArray의 일부를 Tail부터 끝까지 입력
		int ExtraSize = BUFFER_SIZE - m_nTail;
		memcpy(Buff+m_nTail, ByteArray, ExtraSize);

		// 나머지 ByteArray를 배열의 시작부터 입력
		memcpy(Buff, ByteArray+ExtraSize, ByteSize - ExtraSize);
		m_nTail = ByteSize - ExtraSize;
	}
	// 배열이 순환하지 않아도 되는 경우
	else
	{
		// ByteArray를 배열에 입력
		memcpy(Buff+m_nTail, ByteArray, ByteSize);
		m_nTail += ByteSize;
	}

	return ByteSize;
}

int Queue::GetByte(BYTE* ByteArray, int ByteSize)
{
	// 빼올 데이터가 하나도 없을경우
	if (0 == GetSize())
		return FALSE;

	// ByteSize가 출력가능한 Data보다 클 경우
	if (ByteSize > GetSize())
	{
		int QuantityOfData = GetSize();
		// 배열이 순환해야 하는 경우
		if( m_nHead + ByteSize > BUFFER_SIZE - 1 )
		{
			// ByteArray에 배열의 일부를 출력
			int ExtraSize = BUFFER_SIZE - m_nHead;
			memcpy(ByteArray, Buff+m_nHead, ExtraSize);

			// ByteArray의 후위부터 배열의 나머지를 출력
			memcpy(ByteArray+ExtraSize, Buff, QuantityOfData - ExtraSize);
			m_nHead = QuantityOfData - ExtraSize;
		}
		// 배열이 순환하지 않아도 되는 경우
		else
		{
			// ByteArray에 배열을 출력
			memcpy(ByteArray, Buff+m_nHead, QuantityOfData);
			m_nHead += QuantityOfData;
		}

		return QuantityOfData;
	}
	// ByteSize가 출력가능한 Data보다 작을 경우
	else
	{
		// 배열이 순환해야 하는 경우
		if (m_nHead + ByteSize > BUFFER_SIZE - 1)
		{
			// ByteArray에 배열의 일부를 출력
			int ExtraSize = BUFFER_SIZE - m_nHead;
			memcpy(ByteArray, Buff+m_nHead, ExtraSize);

			// ByteArray의 후위부터 배열의 나머지를 출력
			memcpy(ByteArray+ExtraSize, Buff, ByteSize - ExtraSize);
			m_nHead = ByteSize - ExtraSize;
		}
		// 배열이 순환하지 않아도 되는 경우
		else
		{
			// ByteArray에 배열을 출력
			memcpy(ByteArray, Buff+m_nHead, ByteSize);
			m_nHead += ByteSize;
		}

		return ByteSize;
	}
}