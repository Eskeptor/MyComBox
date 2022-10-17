/**
* File				: Comport.h (Comport.cpp)
* Class				: Comport
* Super Class		: Queue
* Function			:
* Latest Update		: 2017/08/10 - j.Noh
*/
#pragma once
#include "stdafx.h"
#include "Queue.h"

constexpr int MAX_BUFF = 4096;

class Comport : public Queue
{
// Enums and structure ========================================================

// ============================================================================


// Variables ==================================================================
private:
	HANDLE m_hComm;
	BOOL m_bReadComm;

	CString m_sPortName;
	DWORD m_dwBaud;
	BYTE m_byData, m_byStop, m_byParity;
	OVERLAPPED m_osRead, m_osWrite;

	BYTE m_byBuff[1024];
	DWORD m_dwRead;
	DWORD m_dwError;
	DWORD m_dwErrorFlags;
	COMSTAT m_comstat;

protected:
	void (Comport::* pRunHandler)(void);

	BOOL m_bConnected;
	BOOL m_bAutoStatus;
	BOOL m_bOrderFlag;

	Queue* m_QueueRead;
	CListBox* m_pListBox;
	CTime m_LogTime;
// ============================================================================


// Functions ==================================================================
protected:
	virtual void Run(void);
	void AutoConnect(void);

	DWORD WriteComm(BYTE* pBuff, DWORD nToWrite);
	void ReadComm(void);

	virtual void OnDataReceived(DWORD dwSize) = 0;
	virtual void OnEventMessage(const CString& Msg) = 0;

public:
	Comport();
	virtual ~Comport();

	void ProcessRun(void);
	BOOL OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity);
	BOOL OpenPort();
	void ClosePort();
	BOOL CheckPortStatus();
	void ResetOrderFlag() { m_bOrderFlag = FALSE; }
	BOOL GetOrderFlag() { return m_bOrderFlag; }

	void SetMessageListBox(CListBox* pListBox) { m_pListBox = pListBox; }

// ============================================================================

};