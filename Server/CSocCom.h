#pragma once

// CSocCom 명령 대상
//통신용 소켓
#define UM_RECEVIE WM_USER+2
#define MAX_CLIENT_COUNT 4 //서버 수용가능 최대 클라이언트 수

#define SOC_CLIENT_CONNECT "접속 성공" //클라이언트가 접속
#define SOC_CLIENT_DISCONNECT "클라이언트 종료" //클라이언트 종료 시 보낼 메시지

class ComSock : public CSocket
{
public:
	ComSock();
	virtual ~ComSock();
	
	HWND m_hWnd;
	void ComSock::Init(HWND hWnd);
	virtual void OnReceive(int nErrorCode);

	int m_index;
};


