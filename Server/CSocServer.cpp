// CSocServer.cpp: 구현 파일
//

#include "pch.h"
#include "Server.h"
#include "CSocServer.h"


// CSocServer

CServerSock::CServerSock()
{
}

CServerSock::~CServerSock()
{
}


// CSocServer 멤버 함수

//소켓 클래스에서는 소켓의 메시지(On으로 시작하는 함수)만을 처리하고, 실제 동작은 메인에서 수행
//소켓 클래스와 메인 윈도우 연결시키는 것 필수 -> Init 함수
void CServerSock::Init(HWND hWnd)
{
	//메인 윈도우 포인터 받기
	m_hWnd = hWnd;
}

//클라이언트에서 접속 요청이 올 경우 OnAccept 함수가 호출됨
//OnAccept 함수가 호출되면 접속 요청할 한 소켓과 다른 소켓 연결위해 Accept함수 호출한 뒤 메인 윈도우에
//OnAccept 함수가 호출되었다는 것 알려줌

void CServerSock::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Accept(m_socCom[m_index.front()]);

	//m_socCom은 연결 요청한 클라이언트 서버와 실제 연결이 되는 소켓
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0);
	CSocket::OnAccept(nErrorCode);
}

//메인 윈도우에서는 m_socCom 얻어서 통신 처리
ComSock* CServerSock::GetAcceptSocCom()
{
	return &m_socCom[m_index.front()];
}
