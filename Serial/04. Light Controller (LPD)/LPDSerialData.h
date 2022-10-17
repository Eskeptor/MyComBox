/**
 * File				: LPDSerialData.h (LPDSerialData.cpp)
 * Class			: CLPDSerialData
 * Super Class		: CSerialBase
 * Function			: 
 * Latest Update	: 2019/03/29 - yc.jeon
 */

#pragma once
#include "SerialBase.h"
#include <vector>

using namespace std;

class CLPDSerialData : public CSerialBase
{
public:
    enum { POS_FULL = 0, POS_CMD, POS_PARAM, POS_FULL_B };
	enum { CMD_C_C_ON = 0, CMD_C_C_OFF, CMD_C_STATUS_READ, CMD_C_ERR_READ,
		   CMD_CBC_C_APPLY, CMD_CBC_STATUS_READ,
		   CMD_BUZ_B_ON, CMD_BUZ_B_OFF,
		   CMD_LOC_L_ON, CMD_LOC_L_OFF };
    const BYTE CR;
    const BYTE LF;

private:
    // Convert CString to BYTE
    void ConvertStringToByte(CString strData);
    // Convert BYTE to CString
    void ConvertByteToString(vector<BYTE>* pVData);
    // Converted Data Split
    void StringSplitForParam(CString* str);
	// Combine CString Vector as a CString
    CString CombineString(vector<CString>* pVData);

public:
    // Parameter Vector
	vector<CString> m_vParams;

    CLPDSerialData(void);
    ~CLPDSerialData(void);

    // Set String Data (Use Only Send Command)
    void SetStringData(CString strData);
    // Set Real(BYTE) Data (Use Only Receive Command)
    void SetRealData(vector<BYTE> vData);
    // Get Real Data to CString
    CString GetRealDataToString(int nType);
};      