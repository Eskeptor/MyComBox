/**
* File				: T3419.h (T3419.cpp)
* Class				: CT3419
* Super Class		: Comport
* Function			:
* Latest Update		: 2021/07/13 - yc.jeon
*/

#pragma once

#define T3419_MAX		1

struct stT3419Data
{
	int nCh;
	double dTemp;
	double dHumid;
};

class CT3419 : public Comport
{
// Enums and structure ========================================================
private:
	enum eT3419
	{
		DataBufferMax = 1024,
		DataSendSize = 8,
		DataRcvSize = 9,

		SendIdx_DAddr = 0,
		SendIdx_ReadR,
		SendIdx_IAddr,
		SendIdx_NReadR = SendIdx_IAddr + 2,
		SendIdx_CRC = SendIdx_NReadR + 2,

		RcvIdx_DAddr = 0,
		RcvIdx_ReadR,
		RcvIdx_NumberByte,
		RcvIdx_Temp,
		RcvIdx_Humid = RcvIdx_Temp + 2,
		RcvIdx_CRC = RcvIdx_Humid + 2,
	};
// ============================================================================


// Variables ==================================================================
private:
	BYTE m_byData[DataBufferMax];
	BOOL m_bMMode;

	stT3419Data m_stData[T3419_MAX];

// ============================================================================


// Functions ==================================================================
private:
	// Make Modbus CRC16
	int MakeModbusCRC16(BYTE* chBuff);

protected:
	virtual void OnDataReceived(DWORD dwSize);
	virtual void OnEventMessage(const CString &Msg);

public:
	CT3419();
	~CT3419();

	// Data Request (Temp, Humid)
	int RequestGetData(int nCh);	

	// Get Data from Data Request
	stT3419Data GetData(int nCh) const;

	void OnManualMode() { m_bMMode = TRUE; }
	void OffManualMode() { m_bMMode = FALSE; }

// ============================================================================

};

