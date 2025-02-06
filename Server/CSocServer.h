#pragma once

#include "CSocCom.h"
#include<list>

#define UM_ACCEPT WM_USER+1// 메인 윈도우에 메시지를 보내기 위한 구별자
// 메인 윈도우에 SendMessage를 이용해 UM_ACCEPT 라는 사용자 정의 메시지를 줌

// CSocServer 명령 대상
//서버용 소켓

class CServerSock : public CSocket
{
public:
	CServerSock();
	virtual ~CServerSock();

	ComSock m_socCom[MAX_CLIENT_COUNT];


	//연결 요청을 한 클라이언트 서버와 실제 연결이 되는 소켓
	ComSock* GetAcceptSocCom(); //통신 소켓 리턴

	std::list<int> m_index;//STL List로 소켓 관리

	HWND m_hWnd;// 메인 윈도우 핸들
	void CServerSock::Init(HWND hWnd);//소켓 클래스와 메인 윈도우를 연결시킴
	virtual void OnAccept(int nErrorCode);//클라이언트 접속 요청 처리
};


