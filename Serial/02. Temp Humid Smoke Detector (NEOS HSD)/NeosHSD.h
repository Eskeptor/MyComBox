/**
* File				: NeosHSD.h (NeosHSD.cpp)
* Class				: CNeosHSD
* Super Class		: Comport
* Function			:
* Latest Update		: 2021/07/13 - yc.jeon
*
* Com Type			: RS-485 (Serial)
* Baudrate			: 9600
* Databit			: 8Bit
* Stopbit			: 1Bit
* Paritybit			: None
* Com Frame			: STX(0x53) + Length(2Byte) + CMD(2Byte) + Code(1Byte) + Count(1Byte) + Data(nByte) + Checksum(2Byte) + ETX(0x45)
*/


#pragma once
#include "Comport.h"

#define NEOS_MAX		4

enum eNeosHSDOp
{
	eNeosOp_Running = 0,	// Running Status
	eNeosOp_Smoke,			// Smoke Detect Status
	eNeosOp_TempWarn,		// Temp Warning Status
	eNeosOp_TempAlarm,		// Temp Alarm Status
	eNeosOp_DetectError,	// Detector Error Status
	eNeosOp_TempWarnSet,	// Temp Warning Setting Status
	eNeosOp_TempWarnKeep,	// Temp Alarm Setting Status
	eNeosOp_TempAlarmKeep,	// Temp Alarm Keep Status
	eNeosOpMax
};

struct stNeosHSDData
{
	int nID;				// Detector ID
	BOOL bOpState[eNeosHSDOp::eNeosOpMax];	// OpState
	int nErrorCode;			// Error Code
	double dTemp;			// Current Temp
	double dMoisture;		// Current Moisture(Humid)
	double dTempWarn;		// Temp Warning Value
	double dTempAlarm;		// Temp Alarm Value
	double dDeviation;		// Deviation

	stNeosHSDData& operator=(const stNeosHSDData& ref)
	{
		nID = ref.nID;
		memcpy(bOpState, ref.bOpState, sizeof(BOOL) * eNeosHSDOp::eNeosOpMax);
		nErrorCode = ref.nErrorCode;
		dTemp = ref.dTemp;
		dMoisture = ref.dMoisture;
		dTempWarn = ref.dTempWarn;
		dTempAlarm = ref.dTempAlarm;
		dDeviation = ref.dDeviation;

		return *this;
	}
};

class CNeosHSD : public Comport
{
// Enums and structure ========================================================
private:
	enum eNeosHSD
	{
		NeosBuffSize = 1024,
		NeosSendCheckSize = 14,
		NeosSendSize = 18,
		NeosRcvDataReqSize = 24,

		SendIdx_STX = 0,
		SendIdx_Length = 1,
		SendIdx_CMD = 3,
		SendIdx_Code = 5,
		SendIdx_Count = 6,
		SendIdx_Data = 7,
		SendIdx_ID = SendIdx_Data,
		SendIdx_Reset = SendIdx_ID + 1,
		SendIdx_WarnSet = SendIdx_Reset + 1,
		SendIdx_AlarmSet = SendIdx_WarnSet + 2,
		SendIdx_Checksum = 15,
		SendIdx_ETX = 17,

		RcvIdx_STX = 0,
		RcvIdx_Length = 1,
		RcvIdx_CMD = 3,
		RcvIdx_Code = 5,
		RcvIdx_Count = 6,
		RcvIdx_Data = 7,
		RcvIdx_ID = RcvIdx_Data,
		RcvIdx_OpState = RcvIdx_Data + 1,
		RcvIdx_ErrorCode = RcvIdx_OpState + 1,
		RcvIdx_Temp = RcvIdx_ErrorCode + 2,
		RcvIdx_Moisture = RcvIdx_Temp + 2,
		RcvIdx_WarnSet = RcvIdx_Moisture + 2,
		RcvIdx_AlarmSet = RcvIdx_WarnSet + 2,
		RcvIdx_DevationSet = RcvIdx_AlarmSet + 2,
		RcvIdx_Checksum = 21,
		RcvIdx_ETX = 23,
	};
// ============================================================================


// Variables ==================================================================
private:
	BYTE m_byData[1024];
	BOOL m_bMMode;
	stNeosHSDData m_stData[NEOS_MAX];

// ============================================================================


// Functions ==================================================================
private:
	virtual void OnDataReceived(DWORD dwSize);
	virtual void OnEventMessage(const CString &Msg);

	int MakeChecksum(BYTE* chBuff);

public:
	CNeosHSD(void);
	virtual ~CNeosHSD(void);

	// Request Data
	int RequestGetData(int nID);
	// Alarm Reset
	int RequestAlarmReset(int nID);
	// Temp Warning, Alarm Base Value Setting
	int RequestSetTempAlarmLimit(int nID, int nWarn, int nAlarm);
	int RequestSetTempAlarmLimit(int nID, double dWarn, double dAlarm);
	// Deviation Setting
	int RequestSetDeviation(int nID, int nDeviation);
	int RequestSetDeviation(int nID, double dDeviation);
	// Temp Warning, Alarm Enable Setting
	int RequestSetAlarmEnable(int nID, BOOL bEnable);
	// Temp Warning Keep Setting
	int RequestSetKeepTempWarn(int nID, BOOL bEnable);
	// Temp Alarm Keep Setting
	int RequestSetKeepTempAlarm(int nID, BOOL bEnable);

	// Get Data from Data Request
	stNeosHSDData GetData(int nID) const;

	// Manual Mode - Listbox Add String On/Off
	void OnManualMode() { m_bMMode = TRUE; }
	void OffManualMode() { m_bMMode = FALSE; }

// ============================================================================

};
