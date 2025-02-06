#pragma once
#include <afxsock.h>
#include "ComSock.h"


// CLevelDlg 대화 상자

class CLevelDlg : public CDialog
{
	DECLARE_DYNAMIC(CLevelDlg)

public:
	CLevelDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLevelDlg();
	
	CComSock* m_ComSock;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
#endif
	enum { IDD = IDD_LEVEL_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnlowLevel();
	afx_msg void OnClickedBtnnormalLevel();
};
