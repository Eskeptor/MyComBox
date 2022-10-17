#include "stdafx.h"
#include "NeosHSD.h"

#define		ClearBit(data, loc)					((data) &= ~(0x1 << (loc)))
#define		ClearBits(data, area, loc)			((data) &= ~(area) << (loc))
#define		SetBit(data, loc)					((data) |= (0x1) << (loc))
#define		SetBits(data, area, loc)			((data) |= ((area) << (loc)))
#define		InvertBit(data, loc)				((data) ^= (0x1) << (loc))
#define		InvertBit(data, area, loc)			((data) ^= ((area) << (loc))
#define		CheckBit(data, loc)					(((data) & (0x1) << (loc)) != 0 ? 1 : 0)
#define		ExtractBits(data, area, loc)		(((data) >> (loc)) & (area))

/**
Constructor
*/
CNeosHSD::CNeosHSD(void)
	: m_byData{0, }, m_bMMode(FALSE)
	, m_stData{0, }
{
	
}


/**
Destructor
*/
CNeosHSD::~CNeosHSD(void)
{
}


/**
Make Checksum
(The sum of 1Byte from Length to Data)
@access		public
@param		chBuff		Data
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::MakeChecksum(BYTE* chBuff)
{
	if (chBuff == nullptr)
		return -1;

	int nResult = 0;
	for (int i = 1; i <= NeosSendCheckSize; i++)
	{
		nResult += chBuff[i];
	}

	return nResult;
}


/**
Data Request
@access		public
@param		nID			Module ID (0~15)
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestGetData(int nID)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x501;
	chBuff[SendIdx_CMD + 1] = 0x501 >> 8;
	chBuff[SendIdx_Code] = 0x01;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	
	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request get data."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}


/**
Alarm Reset
@access		public
@param		nID			Module ID (0~15)
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestAlarmReset(int nID)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x01;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_Reset] = 0x01;

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request alarm reset."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}


/**
Temp Warning, Alarm Base Value Setting
@access		public
@param		nID			Module ID (0~15)
@param		nWarn		Temp Warning Base Value
@param		nAlarm		Temp Alarm Base Value
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestSetTempAlarmLimit(int nID, int nWarn, int nAlarm)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x02;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_WarnSet] = nWarn;
	chBuff[SendIdx_WarnSet + 1] = nWarn >> 8;
	chBuff[SendIdx_AlarmSet] = nAlarm;
	chBuff[SendIdx_AlarmSet + 1] = nAlarm >> 8;

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request set temperature alarm limit."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}
int CNeosHSD::RequestSetTempAlarmLimit(int nID, double dWarn, double dAlarm)
{
	return RequestSetTempAlarmLimit(nID, (int)(dWarn * 100.0), (int)(dAlarm * 100.0));
}


/**
Deviation Setting
@access		public
@param		nID			Module ID (0~15)
@param		nDeviation	Deviation Value
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestSetDeviation(int nID, int nDeviation)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x06;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_WarnSet] = nDeviation;				// Deviation Data Position == Warning Base Value Set Position
	chBuff[SendIdx_WarnSet + 1] = nDeviation >> 8;

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request set deviation."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}
int CNeosHSD::RequestSetDeviation(int nID, double dDeviation)
{
	return RequestSetDeviation(nID, (int)(dDeviation * 100.0));
}


/**
Temp Warning, Alarm Enable Setting
@access		public
@param		nID			Module ID (0~15)
@param		bEnable		Is Enable
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestSetAlarmEnable(int nID, BOOL bEnable)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}
	if (bEnable > 1)
		bEnable = 1;
	else if (bEnable < 0)
		bEnable = 0;

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x03;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_Reset] = bEnable;				

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request set alarm enable."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}


/**
Temp Warning Keep Setting
@access		public
@param		nID			Module ID (0~15)
@param		bEnable		Is Enable
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestSetKeepTempWarn(int nID, BOOL bEnable)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}
	if (bEnable > 1)
		bEnable = 1;
	else if (bEnable < 0)
		bEnable = 0;

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x04;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_Reset] = bEnable;

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request set keeping temperature warning."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}


/**
Temp Alarm Keep Setting
@access		public
@param		nID			Module ID (0~15)
@param		bEnable		Is Enable
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CNeosHSD::RequestSetKeepTempAlarm(int nID, BOOL bEnable)
{
	if (m_bConnected != TRUE)
	{
		OnEventMessage(_T("[NEOS_HSD]Not connected."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[NEOS_HSD]Already doing another request."));
		return -2;
	}
	if (nID < 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid arguments."));
		return 0;
	}
	if (bEnable > 1)
		bEnable = 1;
	else if (bEnable < 0)
		bEnable = 0;

	BYTE chBuff[NeosSendSize] = { 0, };
	chBuff[SendIdx_STX] = 0x53;
	chBuff[SendIdx_Length] = 0x0C;
	chBuff[SendIdx_CMD] = (BYTE)0x503;
	chBuff[SendIdx_CMD + 1] = 0x503 >> 8;
	chBuff[SendIdx_Code] = 0x05;
	chBuff[SendIdx_Count] = 0x00;
	chBuff[SendIdx_ID] = nID;
	chBuff[SendIdx_Reset] = bEnable;

	int nChecksum = MakeChecksum(chBuff);
	if (nChecksum == -1)
	{
		OnEventMessage(_T("[NEOS_HSD]Invalid Checksum."));
		return 0;
	}
	chBuff[SendIdx_Checksum] = nChecksum;
	chBuff[SendIdx_Checksum + 1] = nChecksum >> 8;
	chBuff[SendIdx_ETX] = 0x45;

	int nLength = WriteComm(chBuff, NeosSendSize);
	if (nLength > 0)
	{
		OnEventMessage(_T("[NEOS_HSD]Request set keeping temperature alarm."));
		m_bOrderFlag = TRUE;
	}
	else
		OnEventMessage(_T("[NEOS_HSD]Request failed."));

	return nLength;
}


/**
Get Data from Data Request
@access		public
@param		
@return		m_stData
*/
stNeosHSDData CNeosHSD::GetData(int nID) const
{
	if (nID < NEOS_MAX)
		return m_stData[nID];
	else
		return m_stData[0];
}


/**
Data Receive Event
*/
void CNeosHSD::OnDataReceived(DWORD dwSize)
{
	int nSize = (int)dwSize;
	m_QueueRead->GetByte(m_byData, nSize);

	if (m_byData[RcvIdx_CMD + 1] == 0x05)
	{
		// Data Request Receive
		if (m_byData[RcvIdx_CMD] == 0xA1 &&
			m_byData[RcvIdx_Code] == 0x01 &&
			nSize == NeosRcvDataReqSize)
		{
			int nID = m_byData[RcvIdx_ID];
			m_stData[nID].nID = nID;
			m_stData[nID].bOpState[eNeosOp_Running] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_Running);
			m_stData[nID].bOpState[eNeosOp_Smoke] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_Smoke);
			m_stData[nID].bOpState[eNeosOp_TempWarn] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_TempWarn);
			m_stData[nID].bOpState[eNeosOp_TempAlarm] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_TempAlarm);
			m_stData[nID].bOpState[eNeosOp_DetectError] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_DetectError);
			m_stData[nID].bOpState[eNeosOp_TempWarnSet] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_TempWarnSet);
			m_stData[nID].bOpState[eNeosOp_TempWarnKeep] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_TempWarnKeep);
			m_stData[nID].bOpState[eNeosOp_TempAlarmKeep] = CheckBit(m_byData[RcvIdx_OpState], eNeosOp_TempAlarmKeep);
			m_stData[nID].nErrorCode = m_byData[RcvIdx_ErrorCode];
			memcpy(&m_stData[nID].dTemp, &m_byData[RcvIdx_Temp], sizeof(BYTE) * 2);
			memcpy(&m_stData[nID].dMoisture, &m_byData[RcvIdx_Moisture], sizeof(BYTE) * 2);
			memcpy(&m_stData[nID].dTempWarn, &m_byData[RcvIdx_WarnSet], sizeof(BYTE) * 2);
			memcpy(&m_stData[nID].dTempAlarm, &m_byData[RcvIdx_AlarmSet], sizeof(BYTE) * 2);
			memcpy(&m_stData[nID].dDeviation, &m_byData[RcvIdx_DevationSet], sizeof(BYTE) * 2);

			OnEventMessage(_T("[NEOS_HSD]Rcv get data."));
			m_bOrderFlag = FALSE;
		}
		else if (m_byData[RcvIdx_CMD] == 0xA3)
		{
			// Alarm Reset Receive
			if (m_byData[RcvIdx_Code] == 0x01)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv alarm reset."));
				m_bOrderFlag = FALSE;
			}
			// Temp Warning, Alarm Base Value Setting Receive
			else if (m_byData[RcvIdx_Code] == 0x02)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv set temperature alarm limit."));
				m_bOrderFlag = FALSE;
			}
			// Deviation Setting Receive
			else if (m_byData[RcvIdx_Code] == 0x06)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv set deviation."));
				m_bOrderFlag = FALSE;
			}
			// Temp Warning, Alarm Enable Setting Receive
			else if (m_byData[RcvIdx_Code] == 0x03)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv set alarm enable."));
				m_bOrderFlag = FALSE;
			}
			// Temp Warning Keep Setting Receive
			else if (m_byData[RcvIdx_Code] == 0x04)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv set keeping temperature warning."));
				m_bOrderFlag = FALSE;
			}
			// Temp Alarm Keep Setting Receive
			else if (m_byData[RcvIdx_Code] == 0x05)
			{

				OnEventMessage(_T("[NEOS_HSD]Rcv set keeping temperature alarm."));
				m_bOrderFlag = FALSE;
			}
		}
	}
}

/**
Event Message
*/
void CNeosHSD::OnEventMessage(const CString &strMessage)
{
	CString strMsg = strMessage;
	CString strTime = _T("");

	if (m_pListBox == NULL ||
		m_bMMode == FALSE)
		return;

	m_LogTime = CTime::GetCurrentTime();
	strTime.Format(_T(" [%02d:%02d:%02d] "), m_LogTime.GetHour(), m_LogTime.GetMinute(), m_LogTime.GetSecond());
	strMsg.Format(_T("%s%s"), strTime, strMsg);

	// Forward the strMessage to the listbox to add it.
	::SendMessage(m_pListBox->m_hWnd, LB_INSERTSTRING, 0, (LPARAM)(LPWSTR)(LPCWSTR)strMsg);

	if( 500 < (int)::SendMessage(m_pListBox->m_hWnd, LB_GETCOUNT, 0, 0))
	{
		::SendMessage(m_pListBox->m_hWnd, LB_DELETESTRING, -1, 0);
	}
}