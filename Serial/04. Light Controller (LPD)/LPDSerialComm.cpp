#include "StdAfx.h"
#include "LPDSerialComm.h"

/**
Constructor
*/
CLPDSerialComm::CLPDSerialComm(void)
              : CCommThread()
			  , m_bIsManual(true)
{

}


/**
Destructor
*/
CLPDSerialComm::~CLPDSerialComm(void)
{
	TRACE(_T("LPD Serial Data Delete\n"));
}


/**
 * Convert Data
 */
void CLPDSerialComm::DataConverter()
{
	BYTE aByte;
	vector<BYTE> vData;
	m_bReserveMsg = false;
	int nSize = m_QueueRead.GetSize();
	if (nSize == 0) 
		return;
	for (int i = 0; i < nSize; i++)
	{
		m_QueueRead.GetByte(&aByte);
		
		if (aByte != '\0')
			vData.push_back(aByte);
	}

	DestroyData(false);
	m_data.SetRealData(vData);
	m_data.SetDataType(CLPDSerialData::RECEIVE);

	if (m_bIsManual)
	{
		::SendMessage(g_hWnd_P3_Manual_LPD, UM_MSG_SERIAL_DATA_INCOM, 0, 0);
		::PostMessage(g_hWnd_BridgeCamera, UM_MSG_SERIAL_DATA_INCOM, 0, 0);
	}

	::SetEvent(g_SerialMgr.m_hScomLPD);

	CString strLog = _T("");
	strLog.Format(_T("Light Controller : Command Receive - %s"), m_data.GetRealDataToString(CLPDSerialData::POS_FULL));
	GetLogCtrl()->AddBuf(LOG_SERIAL_COMM, strLog);
}


/**
 * Destroy Data
 * @param	bIsFullDestroy			Is Destroy All Data
 */
void CLPDSerialComm::DestroyData(bool bIsFullDestroy /* = true */)
{
	if (!m_data.m_vParams.empty())
		m_data.m_vParams.clear();

	if (bIsFullDestroy)
	{
		m_nPrevSend = -1;
	}
}


/**
Data Initialize
*/
void CLPDSerialComm::DataInit(int nDataType, int nCmdType, CString strCmd)
{
	DestroyData();
	m_nPrevSend = nCmdType;
	m_data.SetDataType(nDataType);
	m_data.SetStringData(strCmd);
}


/**
 * Get Current Brightness
 * @return	nResult			EVENT_ERR_SEND_FAIL(-2): Fail to send Command
 *							EVENT_ERR_TIMEOUT(-1): Timeout
 *							Positive Value: Success
 */
long CLPDSerialComm::GetLightBrightness()
{
	long lResult = 0;
	CString strCmd = _T("S100");

	TRACE(_T("LPD_GetLightBrightness: Send Command.\n"));

	DataInit(CLPDSerialData::SEND, CLPDSerialData::CMD_CBC_STATUS_READ, strCmd);
	g_SerialMgr.SetManual(CSerialMgr::SCOM_LPD);

	CString strLog = _T("");
	strLog.Format(_T("Light Controller : Command Send - %s"), m_data.GetRealDataToString(CLPDSerialData::POS_FULL));
	GetLogCtrl()->AddBuf(LOG_SERIAL_COMM, strLog);

	lResult = WriteComm(m_data.GetRealDataToByte(), m_data.GetStreamLength() + 1);

	if (lResult == 0)	
		return EVENT_ERR_SEND_FAIL;

	TRACE(_T("LPD_GetLightBrightness: Waiting Respond...\n"));
	DWORD dwResult = ::WaitForSingleObject(g_SerialMgr.m_hScomLPD, RUN_WAIT_TIME);

	if (dwResult == WAIT_TIMEOUT ||
		dwResult == WAIT_FAILED)
	{
		lResult = EVENT_ERR_TIMEOUT;
	}
	else
	{
		TRACE(_T("LPD_GetLightBrightness: Receive Response.\n"));

		CString strResult = m_data.GetRealDataToString(CLPDSerialData::POS_FULL).Mid(2, 2);
		TCHAR* wEnd = NULL;
		lResult = _tcstol(strResult, &wEnd, 16);
	}

	return lResult;
}


/**
 * Set Light On/Off
 * @param	nType			0: RUN_LIGHT_OFF
 *							1: RUN_LIGHT_ON
 * @return	nResult			EVENT_ERR_SEND_FAIL(-2): Fail to send Command
 *							EVENT_ERR_TIMEOUT(-1): Timeout
 *							Positive Value: Success
 */
int CLPDSerialComm::SetLight(int nType)
{
	int nResult = 0;
	CString strCmd = _T("");

	if (nType == RUN_LIGHT_OFF)
		strCmd = _T("H1OF");
	else
		strCmd = _T("H1ON");

	TRACE(_T("LPD_SetLight: Send Command.\n"));

	DataInit(CLPDSerialData::SEND, CLPDSerialData::CMD_C_C_ON, strCmd);
	g_SerialMgr.SetAuto(CSerialMgr::SCOM_LPD);

	CString strLog = _T("");
	strLog.Format(_T("Light Controller : Command Send - %s"), m_data.GetRealDataToString(CLPDSerialData::POS_FULL));
	GetLogCtrl()->AddBuf(LOG_SERIAL_COMM, strLog);

	nResult = WriteComm(m_data.GetRealDataToByte(), m_data.GetStreamLength() + 1);

	if (nResult == 0)		
		return nResult;

	TRACE(_T("LPD_SetLight: Waiting Respond...\n"));
	DWORD dwResult = ::WaitForSingleObject(g_SerialMgr.m_hScomLPD, RUN_WAIT_TIME);

	if (dwResult == WAIT_TIMEOUT ||
		dwResult == WAIT_FAILED)		
	{
		nResult = EVENT_ERR_TIMEOUT;
	}
	else
	{
		TRACE(_T("LPD_SetLight: Receive Response.\n"));
	}
	g_SerialMgr.SetManual(CSerialMgr::SCOM_LPD);

	return nResult;
}
