
// ClientDlg.h: 헤더 파일
//
#include "ComSock.h"
#include "CLevelDlg.h"
#pragma once


// CClientDlg 대화 상자
class CClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	int m_timer_sec = 5;
	
	CComSock m_ComSock;//통신용 소켓

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif
	
private:
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
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);// 소켓 연결 이후 모든 통신을 해당 메시지 함수를 이용
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_chat_list;
	CString m_strIP;
	CString m_strSendText;
	afx_msg void OnConnect();
	afx_msg void OnSend();
	CString m_strUserID;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	CProgressCtrl m_progress;
	void ResetTimer();

	CLevelDlg levelDlg;
	CListBox m_list;
	afx_msg void OnClickedButton1();
	void ResetTimer2();
	void ResetTimer3();
	void ResetTimer4();
};
