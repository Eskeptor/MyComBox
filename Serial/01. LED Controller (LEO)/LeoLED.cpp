#include "stdafx.h"
#include "LeoLED.h"

/**
Constructor
*/
CLeoLED::CLeoLED()
	: m_byData{ 0, }
	, m_bMMode(FALSE)
{
}

/**
Destructor
*/
CLeoLED::~CLeoLED()
{
}


/**
Bright Control
@access		public
@param		nCh			Channel ID
@param		nBright		Bright
@return		-1: Disconnected 
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CLeoLED::RequestChangeBright(int nCh, int nBright)
{
	if (m_bConnected != TRUE) 
	{
		OnEventMessage(_T("[LEO LED]Not connection."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[LEO LED]Already doing another request."));
		return -2;
	}
	if (nCh < 0 ||
		nBright < 0)
	{
		OnEventMessage(_T("[LEO LED]Invalid arguments."));
		return 0;
	}

	CString strSendMsg = _T("");
	strSendMsg.Format(_T("[%02d%03d"), nCh, nBright);

	CStringA strMsgAscii = CStringA(strSendMsg);
	int nLength = WriteComm((unsigned char *)strMsgAscii.GetString(), strMsgAscii.GetLength());
	if (nLength > 0)
	{
		OnEventMessage(_T("[LEO LED]Request change bright."));
		m_bOrderFlag = true;
	}
	else
	{
		OnEventMessage(_T("[LEO LED]Request failed."));
	}

	return nLength;
}


/**
On/Off Control
@access		public
@param		nCh			Channel ID(-1 : All Channel)
@param		bOn			Is On
@return		-1: Disconnected
			-2: Performing other commands
			0: Incorrect arguments
			Positive value: Successed
*/
int CLeoLED::RequestOnOff(int nCh, BOOL bOn)
{
	if (m_bConnected != TRUE) 
	{
		OnEventMessage(_T("[LEO LED]Not connection."));
		return -1;
	}
	if (1 == m_bOrderFlag)
	{
		OnEventMessage(_T("[LEO LED]Already doing another request."));
		return -2;
	}
	if (nCh < -1 ||
		(bOn != TRUE && bOn != FALSE))
	{
		OnEventMessage(_T("[LEO LED]Invalid arguments."));
		return 0;
	}

	CString strSendMsg = _T("");
	if (nCh == -1)
		strSendMsg.Format(_T("AP%d%d"), bOn, bOn);
	else
		strSendMsg.Format(_T("]%02d%d"), nCh, bOn);

	CStringA strMsgAscii = CStringA(strSendMsg);
	int nLength = WriteComm((unsigned char *)strMsgAscii.GetString(), strMsgAscii.GetLength());
	if (nLength > 0)
	{
		OnEventMessage(_T("[LEO LED]Request On Off Setting."));
		m_bOrderFlag = true;
	}
	else
		OnEventMessage(_T("[LEO LED]Request failed."));

	return nLength;
}

/**
Data Received Event
*/
void CLeoLED::OnDataReceived(DWORD dwSize)
{
	int nSize = (int)dwSize;
	m_QueueRead->GetByte(m_byData, nSize);
}

/**
Event Message
*/
void CLeoLED::OnEventMessage(const CString& strMessage)
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

	if (500 < (int)::SendMessage(m_pListBox->m_hWnd, LB_GETCOUNT, 0, 0))
	{
		::SendMessage(m_pListBox->m_hWnd, LB_DELETESTRING, -1, 0);
	}
}
