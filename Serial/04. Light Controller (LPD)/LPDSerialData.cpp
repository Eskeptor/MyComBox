#include "StdAfx.h"
#include "LPDSerialData.h"

/**
Constructor
*/
CLPDSerialData::CLPDSerialData(void)
              : CSerialBase()
			  , CR(0x0D), LF(0x0A)
{

}


/**
Destructor
*/
CLPDSerialData::~CLPDSerialData(void)
{

}


/**
 * Convert CString to BYTE
 * @param	strData		CString Data
 */
void CLPDSerialData::ConvertStringToByte(CString strData)
{
    //TRACE(_T("CLPDSerialData: ConvertStringToByte Start\n"));

	DeleteArray<BYTE>(m_szRealData);

    int nStrLength = strData.GetLength() + 2;       // 2 : CR + LF
    m_szRealData = new BYTE[nStrLength];
    ::ZeroMemory(m_szRealData, nStrLength);

    CStringA strA = CStringA(strData);
    CString strTmp;

    m_strRealData = _T("");

    for (int i = 0; i < nStrLength - 2; i++)
    {
        m_szRealData[i] = (int)strA[i];
        strTmp = ConvertDecToHexToString((int)strA[i], CHAR);
        m_strRealData.Append(strTmp);
    }
    m_szRealData[nStrLength - 2] = CR;
    m_szRealData[nStrLength - 1] = LF;
	m_nStreamLength = nStrLength;

    // String Split
    StringSplitForParam(&strData);

	//TRACE(_T("CLPDSerialData: ConvertStringToByte End\n"));
}


/**
 * Convert BYTE to CString
 * @param	pVData		BYTE Data(Vector)
 */
void CLPDSerialData::ConvertByteToString(vector<BYTE>* pVData)
{
	if (pVData != NULL && !(pVData->empty()))
    {
		//TRACE(_T("CLPDSerialData: ConvertByteToString Start\n"));

        DeleteArray<BYTE>(m_szRealData);			
        m_szRealData = new BYTE[pVData->size()];
        ::ZeroMemory(m_szRealData, pVData->size());

        m_strCommand = _T("");							
        m_strRealData = _T("");							
        m_strStringData = _T("");						
        if (!m_vParams.empty())							
			m_vParams.clear();
            //vector<CString>().swap(m_vParams);

        CString strTmp;
		for (vector<CString>::size_type i = 0; i < pVData->size(); i++)
        {
            m_szRealData[i] = pVData->at(i);               
            strTmp.Format(_T("%02X"), m_szRealData[i]);
            m_strRealData.Append(strTmp);

			if (m_szRealData[i] == 0x0d ||
				m_szRealData[i] == 0x0a)
				continue;

            strTmp.Format(_T("%c"), m_szRealData[i]);
            m_strStringData.Append(strTmp);
        }

        // String Split
        StringSplitForParam(&m_strStringData);

		//TRACE(_T("CLPDSerialData: ConvertByteToString End\n"));
    }
}


/**
 * Converted Data Split
 * @param	str		Data (Command + Parameters)
 */
void CLPDSerialData::StringSplitForParam(CString* str)
{
    // String Split
    //TRACE(_T("CLPDSerialData: String Split Start\n"));

    m_strCommand = (*str)[0];

    if (!m_vParams.empty())	
		m_vParams.clear();
        //vector<CString>().swap(m_vParams);
	CString strStr;
	strStr.AppendChar((*str)[1]);
    m_vParams.push_back(strStr);
	strStr = _T("");
	strStr.AppendChar((*str)[2]);
    m_vParams.push_back(strStr);
	strStr = _T("");
	strStr.AppendChar((*str)[3]);
    m_vParams.push_back(strStr);

    //TRACE(_T("CLPDSerialData: String Split End\n"));
}


/**
 * Set String Data (Use Only Send Command)
 * @param	strData		Send Command
 */
void CLPDSerialData::SetStringData(CString strData)
{
    //TRACE(_T("CLPDSerialData: SetStringData Start\n"));

    SetCurDate(ALL);				
	m_strStringData = strData;		
	ConvertStringToByte(strData);	

	//TRACE(_T("CLPDSerialData: SetStringData End\n"));
}

/**
 * Real Real(BYTE) Data (Use Only Receive Command)
 * @param	vData		Received Data
 */
void CLPDSerialData::SetRealData(vector<BYTE> vData)
{
    //TRACE(_T("CLPDSerialData: SetRealData Start\n"));

    SetCurDate(ALL);				
	ConvertByteToString(&vData);	

	//TRACE(_T("CLPDSerialData: SetRealData End\n"));
}

/**
 * Get Real Data to CString
 * @param	nType		Return Data Type(enum)
 */
CString CLPDSerialData::GetRealDataToString(int nType)
{
    CString strParam;
    switch (nType)
    {
        case POS_FULL:							// Return Command + Parameters
            return m_strStringData;
        case POS_CMD:							// Return Command
            return m_strCommand;
        case POS_PARAM:							// Return Parameters
			return CombineString(&m_vParams);
		case POS_FULL_B:						// Return Command + Parameters (Hexadecimal number)
			return m_strRealData;
    }
	return _T("");
}


/**
 * Combine CString Vector as a CString
 * @param	pVData		Cstring Vector
 * @return	strCombind	Combined CString
 */
CString CLPDSerialData::CombineString(vector<CString>* pVData)
{
	CString strCombind = _T("");
	if (pVData != NULL && !pVData->empty())
	{
		//TRACE(_T("CEGASerialData: CombineString Start\n"));
		for (vector<CString>::size_type i = 0; i < pVData->size(); i++)
			strCombind.Append((*pVData)[i]);
		//TRACE(_T("CEGASerialData: CombineString End\n"));
	}
	return strCombind;
}
