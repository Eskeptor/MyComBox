/**
 * File				: SerialBase.h (SerialBase.cpp)
 * Class			: CSerialBase
 * Super Class		: 
 * Function			: 
 * Latest Update	: 2019/03/06 - yc.jeon
 */

#pragma once
#include <vector>

using namespace std;

class CSerialBase
{
// Enums and structure ========================================================
public:
    enum { RECEIVE = 0, SEND,
           ALL = 0, DATE, YEAR, MONTH, DAY, TIME,
		   NUMERIC = 0, CHAR };
// ============================================================================


// Variables ==================================================================
protected:
	// Data Type (Send, Receive)
	int m_nDataType;
	// Command Type (Command, Event)
	int m_nCommandType;
	// Stream Length (BYTE data length)
	int m_nStreamLength;
	// Data (Real BYTE)
	BYTE* m_szRealData;
	// Data (Real -> CString)
	CString m_strRealData;
	// Data (String Data)
	CString m_strStringData;
	// Command
	CString m_strCommand;
	// Date
	CString m_strDate;

public:
	bool m_bIsManual;
// ============================================================================


// Functions ==================================================================
protected:
	// Convert Decimal to Hex (CString)
    CString ConvertDecToHexToString(int nDec, int nFlag);
	// Convert CString to BYTE
    virtual void ConvertStringToByte(CString strData) = 0;
    // Convert BYTE to CString
	virtual void ConvertByteToString(vector<BYTE>* pVData) = 0;

public:
    CSerialBase();
    virtual ~CSerialBase();

	// Set String Data (Use Only Send Command)
    virtual void SetStringData(CString strData) = 0;
    // eal Real(BYTE) Data (Use Only Receive Command)
	virtual void SetRealData(vector<BYTE> vData) = 0;
	// Set Current Date
	void SetCurDate(int nType);
	// Set Data Type
	void SetDataType(int nType);

	// Get Date (Date : Data Create Time)
    CString GetDate() const;
	// Get String Data
	CString GetStringData() const;
	// Get Real Data to CString
	virtual CString GetRealDataToString(int nType) = 0;
	// Get Real Data (Return BYTE*)
	BYTE* GetRealDataToByte() const;
	// Get Stream Length
	int GetStreamLength() const;
	// Get Data Type
	int GetDataType() const;
	// Reset Data
	void ResetData();

	// Get System Date to CString
    static CString GetSystemDate(int nType);

// ============================================================================


};

