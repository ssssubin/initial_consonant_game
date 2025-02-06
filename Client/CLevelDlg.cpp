// CLevelDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "CLevelDlg.h"
#include "afxdialogex.h"
#include "ClientDlg.h"
#include "CClient2.h"

// CLevelDlg 대화 상자

IMPLEMENT_DYNAMIC(CLevelDlg, CDialog)

CLevelDlg::CLevelDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LEVEL_DIALOG, pParent)
{

}

CLevelDlg::~CLevelDlg()
{
}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialog)
	ON_BN_CLICKED(IDC_btnLOW_LEVEL, &CLevelDlg::OnBnClickedbtnlowLevel)
	ON_BN_CLICKED(IDC_btnNORMAL_LEVEL, &CLevelDlg::OnClickedBtnnormalLevel)
END_MESSAGE_MAP()


// CLevelDlg 메시지 처리기
void CLevelDlg::OnBnClickedbtnlowLevel()
{
	CClientDlg dlg;
	dlg.DoModal();

}

void CLevelDlg::OnClickedBtnnormalLevel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClient2 dlg;
	dlg.DoModal();
}
