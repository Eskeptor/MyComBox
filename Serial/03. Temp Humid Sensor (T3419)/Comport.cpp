#include "stdafx.h"
#include "Comport.h"

/**
Constructor
*/
Comport::Comport()
{
	m_hComm = NULL;
	m_bReadComm = FALSE;

	// Comport Connect Init
	m_sPortName = Const::STRING_EMPTY;
	m_dwBaud = NULL;
	m_byData = NULL;
	m_byStop = NULL;
	m_byParity = NULL;

	pRunHandler = NULL;

	m_bConnected = FALSE;
	m_bAutoStatus = FALSE;
	m_bOrderFlag = FALSE;

	m_pListBox = NULL;
	m_QueueRead = NULL;

	memset(m_byBuff, 0, 1024);
}

/**
Destructor
*/
Comport::~Comport()
{
	if( m_QueueRead != NULL ) 	
	{
		m_QueueRead->Clear();
		delete m_QueueRead;
		m_QueueRead = NULL;
	}
	m_bConnected = FALSE;
	m_bAutoStatus	= FALSE;
	m_bOrderFlag = FALSE;
	m_bReadComm = FALSE;

	pRunHandler = NULL;

	SetCommMask( m_hComm, 0);
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	CloseHandle(m_hComm);
	m_hComm = NULL;
	m_pListBox = NULL;
}

void Comport::ProcessRun( void )
{
	if( pRunHandler != NULL ) 
		Run();
	else 
		pRunHandler = &Comport::AutoConnect;
}

void Comport::Run( void )
{
	(this->*pRunHandler)();
}

void Comport::AutoConnect( void )
{
	if(CheckPortStatus())
	{
		pRunHandler = &Comport::ReadComm;
		return;
	}
	else
	{
		if(m_bAutoStatus)
		{
			OpenPort();
		}
		pRunHandler = &Comport::AutoConnect;
	}
}

/**
Port Open
@param		strPortName		Port Name
@param		dwBaud			Baudrate
@param		byData			DataBit
@param		byStop			StopBits
@param		byParity		Parity
*/
BOOL Comport::OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity)
{
	if (strPortName.ReverseFind('\\') > 0)
	{
		m_sPortName = strPortName;
	}
	else
	{
		m_sPortName = _T("\\\\.\\") + strPortName;
	}

	m_dwBaud = dwBaud;
	m_byData = byData;
	m_byStop = byStop;
	m_byParity = byParity;

	return OpenPort();
}

/**
Port Open
*/
BOOL Comport::OpenPort()
{
	if (m_sPortName.IsEmpty())
	{
		OnEventMessage(_T("There is an empty item(port) in the required data.Please check again."));
		return FALSE;
	}

	if (m_dwBaud == NULL)
	{
		OnEventMessage(_T("There is an empty item(baud rate) in the required data.Please check again."));
		return FALSE;
	}

	if (m_byData == NULL)
	{
		OnEventMessage(_T("There is an empty item(data bit) in the required data.Please check again."));
		return FALSE;
	}

	if (CheckPortStatus())
	{
		OnEventMessage(_T("Serial port is already connected."));
		return FALSE;
	}

	if (m_bConnected)
	{
		OnEventMessage(_T("Port connection failed. The port is already connected."));
		return FALSE;
	}

	if (m_QueueRead == NULL) 
	{
		m_QueueRead = new Queue;
	}

	// Local Parameter
	COMMTIMEOUTS timeouts{};
	DCB dcb{};

	// Initializing overlapped structure
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh	= 0;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;

	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) 	
	{
		OnEventMessage(_T("Failed to create Read Event."));
		return FALSE;
	}

	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		OnEventMessage(_T("Failed to create Write Event."));
		return FALSE;
	}

	m_hComm = CreateFile(m_sPortName, 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL,
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL
		);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		OnEventMessage(_T("Failed to create serial port. Ports that are already connected or do not match."));
		return FALSE;
	}

	// Setting Port State
	SetCommMask(m_hComm, EV_RXCHAR);
	SetupComm(m_hComm, BUFFER_SIZE, BUFFER_SIZE);	
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// Setting Timeout
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / m_dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( m_hComm, &timeouts);

	// Setting DCB
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);	
	dcb.BaudRate = m_dwBaud;
	dcb.ByteSize = m_byData;
	dcb.Parity = m_byParity;
	dcb.StopBits = m_byStop;

	if (!SetCommState(m_hComm, &dcb))	
	{
		OnEventMessage(_T("Comport DCB state setting Failed."));
		return FALSE;
	}

	pRunHandler = &Comport::ReadComm;

	m_bConnected = TRUE;
	m_bAutoStatus = TRUE;
	m_bReadComm = TRUE;

	OnEventMessage(_T("Port has successfully connected."));

	return TRUE;
}

/**
Port Close
*/
void Comport::ClosePort()
{
	if (m_QueueRead != NULL) 	
	{
		delete m_QueueRead;
		m_QueueRead = NULL;
	}

	if (!m_bConnected) 
	{
		OnEventMessage(_T("Already disconnected."));
		return;
	}

	OnEventMessage(_T("Disconnect."));

	m_bConnected = FALSE;
	m_bAutoStatus	= FALSE;
	m_bOrderFlag = FALSE;
	m_bReadComm = FALSE;

	pRunHandler = NULL;

	SetCommMask(m_hComm, 0);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	CloseHandle(m_hComm);
	m_hComm = NULL;
}

/**
Data Write
@param		pBuff		Data Buffer
@param		nToWrite	Data Length
*/
DWORD Comport::WriteComm(BYTE* pBuff, DWORD nToWrite)
{
	// Local Parameter
	DWORD		dwWritten;
	DWORD		dwError;
	DWORD		dwErrorFlags;
	COMSTAT		comstat;

	if (!m_bConnected)
	{
		return 0;
	}

	// Write Data
	if (!WriteFile(m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		// Remained writing Data
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (!GetOverlappedResult(m_hComm, &m_osWrite, &dwWritten, true))
			{
				dwError = GetLastError();

				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(m_hComm, &dwErrorFlags, &comstat);
		}
	}

	return dwWritten;
}

/**
Read Data
*/
void Comport::ReadComm()
{
	if (m_QueueRead == NULL)
	{
		return;
	}

	// Delete Error Flag
	ClearCommError( m_hComm, &m_dwErrorFlags, &m_comstat);

	// Reading System queue if it can
	m_dwRead = m_comstat.cbInQue;

	if (m_dwRead > 0)
	{
		if(!ReadFile(m_hComm, m_byBuff, 2048, &m_dwRead, &m_osRead))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				while (!GetOverlappedResult(m_hComm, &m_osRead, &m_dwRead, TRUE))
				{
					m_dwError = GetLastError();

					if (m_dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hComm, &m_dwErrorFlags, &m_comstat);
						break;
					}
				}
			}
			else
			{
				m_dwRead = 0;
				ClearCommError(m_hComm, &m_dwErrorFlags, &m_comstat);
			}
		}
		else
		{
			if (m_QueueRead != NULL)
			{
				if (BUFFER_SIZE - m_QueueRead->GetSize() > (int)m_dwRead)
				{
					m_QueueRead->PutByte(m_byBuff, m_dwRead);
					OnDataReceived(m_dwRead);
				}
				else
				{
					OnEventMessage(_T("The buffer is full and initialized."));
					m_QueueRead->Clear();
				}
			}
		}
	}
	pRunHandler = &Comport::AutoConnect;
}

/**
Check Port Status
*/
BOOL Comport::CheckPortStatus()
{
	if (m_hComm != NULL &&
		m_bReadComm == TRUE &&
		m_bConnected == TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
