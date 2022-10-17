/**
* File				: LeoLED.h (LeoLED.cpp)
* Class				: CLeoLED
* Super Class		: Comport
* Function			:
* Latest Update		: 2021/07/13 - yc.jeon
*/

#pragma once

class CLeoLED : public Comport
{
// Enums and structure ========================================================
private:
	enum eLeoLED
	{
		DataBufferMax = 1024,
		ChannelMax = 4,
	};
// ============================================================================


// Variables ==================================================================
private:
	BYTE m_byData[DataBufferMax];
	BOOL m_bMMode;

// ============================================================================


// Functions ==================================================================
protected:
	virtual void OnDataReceived(DWORD dwSize);
	virtual void OnEventMessage(const CString &Msg);

public:
	CLeoLED();
	~CLeoLED();

	// Bright Control
	int RequestChangeBright(int nCh, int nBright);
	// On/Off Control
	int RequestOnOff(int nCh, BOOL bOn);

	void OnManualMode() { m_bMMode = TRUE; }
	void OffManualMode() { m_bMMode = FALSE; }

// ============================================================================

};

