// SocCom.cpp: 구현 파일
//

#include "pch.h"
#include "Client.h"
#include "ComSock.h"

// CSocCom

CComSock::CComSock()
{
}

CComSock::~CComSock()
{
}


// CSocCom 멤버 함수
//소켓클래스와 메인 윈도우 연결
void CComSock::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

//데이터 도착 알려주는 가상함수
void CComSock::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SendMessage(m_hWnd, UM_RECEIVE, 0, 0);
	CSocket::OnReceive(nErrorCode);
}



