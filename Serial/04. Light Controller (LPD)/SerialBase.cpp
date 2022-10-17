#include "stdafx.h"
#include "SerialBase.h"

/**
Constructor
*/
CSerialBase::CSerialBase() 
            : m_nDataType(SEND)
			, m_nCommandType(-1)
			, m_nStreamLength(0)
			, m_szRealData(NULL)
			, m_bIsManual(false)
{

}


/**
Destructor
*/
CSerialBase::~CSerialBase()
{
	TRACE(_T("Serial Array Data Delete\n"));
    DeleteArray<BYTE>(m_szRealData);
}


/**
 * Convert Decimal to Hex (CString)
 * @param	nDec			Decimal
 * @param	nFlag			0: Decimal is NUMERIC
 *							1: Decimal is CHAR
 * @return	strReturn		Converted to Hex
 */
CString CSerialBase::ConvertDecToHexToString(int nDec, int nFlag)
{
    CString strReturn;

	if (nFlag == NUMERIC)
	{
		TCHAR chHex[9] = { 0, };
		int nRoot = nDec;
		int nCur = 0;

		for (int i = 7; i >= 0; i--)
		{
			nCur = nRoot % 16;
			nRoot /= 16;
			if (nCur < 10)
				chHex[i] = '0' + nCur;
			else
				chHex[i] = 'A' + nCur - 10;
		}
		strReturn.Format(_T("%s"), chHex);
	}
	else
	{
		TCHAR chHex[3] = { 0, };
		int nRoot = nDec;
		int nCur = 0;

		for (int i = 1; i >= 0; i--)
		{
			nCur = nRoot % 16;
			nRoot /= 16;
			if (nCur < 10)
				chHex[i] = '0' + nCur;
			else
				chHex[i] = 'A' + nCur - 10;
		}
		strReturn.Format(_T("%s"), chHex);
	}

	return strReturn;
}


/**
 * Set Current Date
 * @param	nType			Date Type(enum)
 *							(ALL, DATE, YEAR, MONTH, DAY, TIME)
 */
void CSerialBase::SetCurDate(int nType)
{
    m_strDate = GetSystemDate(nType);
}


/**
 * Set Data Type
 * @param	nType			Data Type(enum)
 *							(RECEIVE, SEND)
 */
void CSerialBase::SetDataType(int nType)
{
    m_nDataType = nType;
}


/**
 * Get Date (Date : Data Create Time)
 * @return	m_strDate		Date
 */
CString CSerialBase::GetDate() const
{
    return m_strDate;
}


/**
 * Get String Data
 * @return	m_strStringData	String Data
 */
CString CSerialBase::GetStringData() const
{
    return m_strStringData;
}


/**
 * Get Real Data (Return BYTE*)
 * @return	m_szRealData	BYTE Data
 */
BYTE* CSerialBase::GetRealDataToByte() const
{
    return m_szRealData;
}


/**
 * Get Stream Length
 * @return	m_nStreamLength	Stream Length
 */
int CSerialBase::GetStreamLength() const
{
    return m_nStreamLength;
}


/**
 * Get Data Type
 * @return	m_nDataType		Data Type
 */
int CSerialBase::GetDataType() const
{
    return m_nDataType;
}


/**
 * Reset Data
 * @return
 */
void CSerialBase::ResetData()
{
	m_strCommand = _T("");
}


/**
 * Get System Date to CString
 * @param	nType			Date Type(enum)
 *							(ALL, DATE, YEAR, MONTH, DAY, TIME)
 * @return	strTime			System Date
 */
CString CSerialBase::GetSystemDate(int nType)
{
    CString strTime;
	SYSTEMTIME time;
	GetLocalTime(&time);

	if ((nType == ALL) || (nType == DATE) || (nType == YEAR))
		strTime.Format(_T("%d"), time.wYear);

	CString strTmp;
	if ((nType == ALL) || (nType == DATE) || (nType == MONTH))
	{
		if (time.wMonth < 10)
			strTmp.Format(_T("0%d"), time.wMonth);
		else
			strTmp.Format(_T("%d"), time.wMonth);
		strTime += strTmp;
	}

	if ((nType == ALL) || (nType == DATE) || (nType == DAY))
	{
		if (time.wDay < 10)
			strTmp.Format(_T("0%d"), time.wDay);
		else
			strTmp.Format(_T("%d"), time.wDay);
		strTime += strTmp;
	}

	if ((nType == ALL) || (nType == TIME))
	{
		if (time.wHour < 10)
			strTmp.Format(_T("0%d"), time.wHour);
		else
			strTmp.Format(_T("%d"), time.wHour);
		strTime += strTmp;
	}

	if ((nType == ALL) || (nType == TIME))
	{
		if (time.wMinute < 10)
			strTmp.Format(_T("0%d"), time.wMinute);
		else
			strTmp.Format(_T("%d"), time.wMinute);
		strTime += strTmp;
	}

	if ((nType == ALL) || (nType == TIME))
	{
		if (time.wSecond < 10)
			strTmp.Format(_T("0%d"), time.wSecond);
		else
			strTmp.Format(_T("%d"), time.wSecond);
		strTime += strTmp;
	}
	return strTime;
}