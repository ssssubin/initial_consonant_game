#pragma once

#include "CComSock2.h"
#include "CLevelDlg.h"

// CClient2 대화 상자


class CClient2 : public CDialogEx
{
	DECLARE_DYNAMIC(CClient2)

public:
	CClient2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CClient2();
 
	CComSock2 m_ComSock2;//통신용 소켓
	int m_timer_sec1 = 5;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif
private:
	protected:

		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


protected:
	HICON m_hIcon;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CListBox m_chat_list1;
	CListBox m_list1;
	CString m_strUserID;
	CString m_strIP;
	CString m_strSendText;
	afx_msg void OnSend1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedButton1();
	afx_msg void OnConnect1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_progress1;
	void ResetTimer1();
	
	void ResetTimer2();
	void ResetTimer3();
	void ResetTimer4();
};
