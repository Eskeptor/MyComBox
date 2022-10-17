/**
 * File				: LPDSerialData.h (LPDSerialData.cpp)
 * Class			: CLPDSerialComm
 * Super Class		: CCommThread
 * Function			: 
 * Latest Update	: 2019/07/10 - yc.jeon
 *
 * Baudrate  		: 9600
 * Databit      	: 8bit
 * StopBits         : 1bit
 * Parity      		: None
 */

#pragma once
#include "CommThread.h"
#include "LPDSerialData.h"

class CLPDSerialComm : public CCommThread
{
// Enums and structure ========================================================
public:
	enum { RUN_WAIT_TIME = 10000 };
	enum { EVENT_ERR_TIMEOUT = -1, EVENT_ERR_SEND_FAIL = -2 };
	enum { RUN_LIGHT_OFF = 0, RUN_LIGHT_ON};
// ============================================================================


// Variables ==================================================================
public:
	bool m_bIsManual;
	int m_nPrevSend;

	CLPDSerialData m_data;
// ============================================================================


// Functions ==================================================================
private:
	// Convert Data
	void DataConverter();

public:
    CLPDSerialComm(void);
    ~CLPDSerialComm(void);

	// Get Current Brightness
	long GetLightBrightness();
	// Set Light On/Off
	int SetLight(int nType);

	// Data Initialize
	void DataInit(int nDataType, int nCmdType, CString strCmd);
	// Destroy Data
	void DestroyData(bool bIsFullDestroy = true);
// ============================================================================

};