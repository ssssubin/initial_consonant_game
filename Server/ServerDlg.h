
// ServerDlg.h: 헤더 파일
//

#pragma once

#include "stdAfx.h"

// CServerDlg 대화 상자
class CServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CServerSock m_ServerSock; //서버용 소켓
	ComSock* m_ComSock[MAX_CLIENT_COUNT]; //통신용 소켓
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);//클라이언트 접속 요청이 왔을 때 실행되는 메시지 함수
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);//클라이언트에서 오는 데이터 수신할 때 실행되는 메시지 함수
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSend;
	CString m_strStatus;
	CListBox m_chat_list;
	afx_msg void OnSend();

	std::list<int> m_using;
	CStatic m_ctlchosung;
	CString strTotal;
	afx_msg void OnBtnChosung();
	char c1[100];
	CString racho;
	CString end1="";
	void Separatecho(const char* s, char* cho);
	CListBox m_list;
	afx_msg void OnClickedBtnChosung2();
	afx_msg void OnBnClickedTimerStr();
	afx_msg void OnBnClickedTimerEnd();
	void RandomNum();
	bool Repeated(CString str, CString str1);
	CStringArray array;
	afx_msg void OnBnClickedTimerStr1();
	afx_msg void OnBnClickedTimerStr2();
	afx_msg void OnBnClickedTimerStr3();
	afx_msg void OnBnClickedTimerEnd1();
	afx_msg void OnBnClickedTimerEnd2();
	afx_msg void OnBnClickedTimerEnd3();
};
