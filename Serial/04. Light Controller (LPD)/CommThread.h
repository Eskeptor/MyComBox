﻿/**
* File				: CommThread.h (CommThread.cpp)
* Class				: CCommThread
* Super Class		: 
* Function			:
* Latest Update		: http://blog.daum.net/pg365/276
*/

//---------------------------- 상수 정의 --------------------------//
#pragma once
#define	BUFF_SIZE		8196
#define	WM_COMM_READ	(WM_USER + 1)		

#define	ASCII_LF		0x0a
#define	ASCII_CR		0x0d
#define	ASCII_XON		0x11
#define	ASCII_XOFF		0x13

// Queue 클래스 정의 //
class	CQueue
{
public:
	BYTE	buff[BUFF_SIZE];
	int 	m_iHead, m_iTail;
	CQueue();
	void	Clear();
	int		GetSize();
	BOOL	PutByte(BYTE b);		// 1 byte 넣기
	BOOL	GetByte(BYTE *pb);		// 1 byte 꺼내기
};


//	통신 클래스	CCommThread 

// 포트 열기 : OpenPort("COM1", CBR_9600);
// 포트에서 읽기 :
//   포트를 연 후에 포트에 자료가 도착하면 WM_COMM_READ 메시지가 메인 
//   윈도우에 전달된다. ON_MESSAGE 매크로를 이용, 함수를 연결하고
//   m_ReadData String에 저장된 데이터를 이용 읽기
// 포트에 쓰기 : WriteComm(buff, 30)과 같이 버퍼와 그 크기를 건네면 된다.

class	CCommThread
{
public:
	CCommThread();
	//--------- 환경 변수 -----------------------------------------//
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.

	//--------- 통신 버퍼 -----------------------------------------//
	CQueue	m_QueueRead;

	//--------- 외부 사용 함수 ------------------------------------//
	BOOL OpenPort(CString m_stPort, CString m_stBaud,CString m_stDatabit,
						   	CString m_stFlow,CString m_stParity,CString m_stStop);
	void	ClosePort();
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);

	//--------- 메세지 전달 확인용 ------------------------------------//
	BOOL m_bReserveMsg;

	//--------- 메세지 전달 타겟 ------------------------------------//
	HWND hCommWnd;

	//--------- 포트를 얻어오는 함수 --------------------------------//
	CStringArray* GetPorts();

	// 작성자 : 전예찬
	virtual void DataConverter() = 0;
};

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CCommThread* pComm);
