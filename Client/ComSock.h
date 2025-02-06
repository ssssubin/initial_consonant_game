#pragma once

// CSocCom 명령 대상
//통신용 소켓
#include<afxsock.h>//소켓 클래스 사용위해 include

#define UM_RECEIVE WM_USER+2

#define S0C_CLIENT_CONNECT "접속성공"//클라이언트가 접속
#define SOC_CLIENT_DISCONNECT "클라이언트 종료"//클라이언트 종료 시 보낼 메시지

class CComSock : public CSocket
{
public:
	CComSock();
	virtual ~CComSock();

	HWND m_hWnd;//메인윈도우 핸들
	void CComSock::Init(HWND hWnd);//소켓 클래스와 메인 윈도우 연결
	virtual void OnReceive(int nErrorCode);//데이터 도착알림

	int m_index; //채팅 클라이언트 번호
};


