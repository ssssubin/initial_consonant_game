
// ServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "time.h"
#include "Afxcoll.h"
#include "stdlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg 대화 상자



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_strSend(_T(""))
	, m_strStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEND_TEXT, m_strSend);
	DDX_Control(pDX, IDC_CHAT_LIST, m_chat_list);
	DDX_Control(pDX, IDC_CHOSUNG, m_ctlchosung);
	DDX_Control(pDX, IDC_CLIENT_SEQ, m_list);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_SEND, &CServerDlg::OnSend)
	ON_BN_CLICKED(IDC_BTN_CHOSUNG, &CServerDlg::OnBtnChosung)
	ON_BN_CLICKED(IDC_BTN_CHOSUNG2, &CServerDlg::OnClickedBtnChosung2)
	ON_BN_CLICKED(IDC_TIMER_STR, &CServerDlg::OnBnClickedTimerStr)
	ON_BN_CLICKED(IDC_TIMER_END, &CServerDlg::OnBnClickedTimerEnd)
	ON_BN_CLICKED(IDC_TIMER_STR1, &CServerDlg::OnBnClickedTimerStr1)
	ON_BN_CLICKED(IDC_TIMER_STR_2, &CServerDlg::OnBnClickedTimerStr2)
	ON_BN_CLICKED(IDC_TIMER_STR3, &CServerDlg::OnBnClickedTimerStr3)
	ON_BN_CLICKED(IDC_TIMER_END1, &CServerDlg::OnBnClickedTimerEnd1)
	ON_BN_CLICKED(IDC_TIMER_END2, &CServerDlg::OnBnClickedTimerEnd2)
	ON_BN_CLICKED(IDC_TIMER_END_3, &CServerDlg::OnBnClickedTimerEnd3)
END_MESSAGE_MAP()


// CServerDlg 메시지 처리기

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	for (int i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		m_ServerSock.m_index.push_back(i);
	}
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//서버 소켓을 생성(포트 번호 7777)
	m_ServerSock.Create(7777);
	//클라이언트 접속 기다림
	m_ServerSock.Listen();
	//OnBtnChosung();
	//소켓 클래스와 메인 윈도우를 연결
	m_ServerSock.Init(this->m_hWnd);
	
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//클라이언트 연결 요청이 왔기 때문에 Accept 함수로 접속
//실제 접속을 담당하는 것-> CServerSock
//이렇게 접속한 소켓은 GetAcceptSocCom을 이용해 얻어옴
//OnAccept 실행 이후 서버용 소켓인 m_socServer의 역할 끝나고
//실제 모든 통신은 통신용 소켓인 m_socCom 이용
//클라이언트 접속 요청 처리

LPARAM CServerDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	//클라이언트에서 접속 요청이 왔을 때
	int tmp = m_ServerSock.m_index.front();

	try
	{
		//통신용 소켓을 생성한 뒤
		CString number; //클라이언트 번호
		number.Format("%d", tmp);// int형을 문자열로 변환
		CString str;

		m_ComSock[tmp] = new ComSock();
		//서버소켓과 통신소켓을 연결

		m_ComSock[tmp] = m_ServerSock.GetAcceptSocCom();

		m_ServerSock.m_index.pop_front();;
		m_using.push_back(tmp);


		m_ComSock[tmp]->m_index = tmp;
		
		m_ComSock[tmp]->Init(this->m_hWnd);

		int i = m_chat_list.GetCount();//listbox의 데이터의 갯수
		
		//클라이언트에게 연결 성공 메시지를 보낼때 클라이언트 번호도 같이 보냄
		m_ComSock[tmp]->Send((SOC_CLIENT_CONNECT + (number)), 256);

		m_chat_list.InsertString(i, ("참여자") + number + "접속");
		
	
	
		//클라이언트 접속 시 접속했다는 메시지 띄움

		
	}
	catch (CException* ex)
	{
		ex->ReportError();
	}
	//
	if (tmp == 3)
	{
		int i = m_chat_list.GetCount();
		RandomNum();
		m_chat_list.InsertString(i, ("게임 시작"));
		for (tmp = 0; tmp < 4; tmp++)
		{
			m_ComSock[tmp]->Send("게임 시작", 256);
		}
	}

	UpdateData(FALSE);
	return TRUE;
}

//데이터 보내는 것은 소켓 클래스의 멤버함수인 Send 이용
//데이터 받을 때는 통신 소켓 클래스에 오버라이딩한 OnReceive 메시지 함수 사용
//데이터가 도착했다는 것을 알려줌

LPARAM CServerDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	//접속된 곳에서 데이터 도착했을 때
	char data[256];
	CString str;
	memset(data, '\0', 256);
	char cho_str[101] = "";
	int i = m_chat_list.GetCount();

	//데이터를 data에 받음
	m_ComSock[wParam]->Receive(data, 256);//wParam = 클라이언트 번호
	str.Format(_T("%s"), data);

	if (str.Compare(SOC_CLIENT_DISCONNECT) == 0)
	{
		m_ServerSock.m_socCom[wParam].Close();
		m_ComSock[wParam]->Close();
		m_using.erase(std::remove(m_using.begin(), m_using.end(), wParam),
			m_using.end());

	}

	else
	{
		//리스트박스에 보여줌
		CString id;
		id.Format("%d", wParam);

		if (str == "1")
		{
			m_chat_list.InsertString(i, "참여자 0 시간 초과 탈락");
			for each (int i in m_using)
			{
				m_ComSock[i]->Send(("참여자 " + id + " 시간 초과 탈락"), 256);
			}
			goto END;

		}
		
		else if (str == "2")
		{
			m_chat_list.InsertString(i, "참여자 1 시간 초과 탈락");
			for each (int i in m_using)
			{
				m_ComSock[i]->Send(("참여자 " + id + " 시간 초과 탈락"), 256);
			}
			
			goto END;
		}
		else if (str == "3")
		{
			m_chat_list.InsertString(i, "참여자 2 시간 초과 탈락");
			for each (int i in m_using)
			{
				m_ComSock[i]->Send(("참여자 " + id + " 시간 초과 탈락"), 256);
			}
			goto END;
		}
		else if (str == "4")
		{
			m_chat_list.InsertString(i, "참여자 3 시간 초과 탈락");
			for each (int i in m_using)
			{
				m_ComSock[i]->Send(("참여자 " + id + " 시간 초과 탈락"), 256);
			}
			goto END;
		}


		int i = m_chat_list.GetCount();
		m_chat_list.InsertString(i, ("참여자" + id + +" : " + str));
		//전송
		for each (int i in m_using)
		{
			if (i !=_ttoi(id))
			{
				m_ComSock[i]->Send(("참여자" + id + " : " + str), 256);
			}
		}

		
		Separatecho(str, cho_str);
		
		if (racho.CompareNoCase(cho_str) == 0)
		{
			i++;
			m_chat_list.InsertString(i, "초성 일치");
			array.Add(" ");
			for (int j = 0; j < array.GetCount(); j++)
			{
				if (Repeated(array[j], str) == FALSE)
				{
					for (int n = 0; n < 4; n++)
					{
						m_ComSock[n]->Send(("참여자" + id + " 단어 반복으로 탈락"), 256);
					}
					i++;
					m_chat_list.InsertString(i, "참여자" + id + " 단어 반복으로 탈락");
					break;
				}
			}
			array.Add(str);

		}
		else
		{
			for each (int i in m_using)
			{
				m_ComSock[i]->Send(("참여자 " + id + " 초성 불일치 탈락"), 256);
			}
			i++;
			m_chat_list.InsertString(i, "참여자 " + id + " 초성 불일치 탈락");
			m_chat_list.InsertString(i, "초성 불일치");
		}
	END:;
	}

	return TRUE;
}


//전송 버튼 클릭할 때
void CServerDlg::OnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	char data[256];
	CString str;

	//data에 전송할 데이터 입력
	memset(data, '\0', 256);
	strcpy_s(data,"사회자 : " + m_strSend);
	m_strSend = "";

	//전송
	for each (int i in m_using)
	{
		m_ComSock[i]->Send(data, 256);
	}

	//전송한 데이터도 리스트박스에 보여줌
	str.Format(_T("%s"), data);//int형을 문자열로 반환
	int i = m_chat_list.GetCount();//listbox의 데이터의 갯수
	m_chat_list.InsertString(i, str);

	UpdateData(FALSE);
}


void CServerDlg::OnBtnChosung()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	UpdateData(TRUE);
	srand(time(NULL));
	char* cho_list[] = { "ㄱ", "ㄴ", "ㄷ", "ㄹ", "ㅁ","ㅂ", "ㅅ", "ㅇ", "ㅈ", "ㅊ", "ㅋ","ㅌ", "ㅍ", "ㅎ"};
	char data[256];
	
	
	int ra = rand() % 14;
	int ra1 = rand() % 14;
	
	char c2[5];
	
	strcpy_s(c1, cho_list[ra]);//c1에 cho_list의 초성 중 하나 랜덤으로 복사
	strcpy_s(c2, cho_list[ra1]);

	
	strcat_s(c1, c2);//c1 문자열 뒤에 c2 문자열 연결
	
	racho = c1;
	
	m_ctlchosung.SetWindowText(racho);//static text에 초성 출력

	//클라이언트 리스트 박스에도 초성 출력
	strcpy_s(data, "사회자 : " + racho);
	for each (int i in m_using)
	{
		m_ComSock[i]->Send(data, 256);
	}

	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, racho);
	
}


void CServerDlg::Separatecho(const char* s, char* cho)
{
	int k, j, cnt;
	int cho_index;
	char han[3] = { 0, };
	static char* cho_list[] = { "",  "ㄱ", "ㄴ", "ㄷ", "ㄹ", "ㅁ","ㅂ", "ㅅ", "ㅇ", "ㅈ", "ㅊ", "ㅋ","ㅌ", "ㅍ", "ㅎ" };

	for (k = j = cnt=0; s[k] && k < 100; k++)
	{
		han[0] = s[k++];
		han[1] = s[k];

		if (strcmp("가", han) <= 0 && strcmp("나", han) > 0)
		{
			cho_index = 1;
		}
		else if (strcmp("다", han) > 0) cho_index = 2;
		else if (strcmp("라", han) > 0) cho_index = 3;
		else if (strcmp("마", han) > 0) cho_index = 4;
		else if (strcmp("바", han) > 0) cho_index = 5;
		else if (strcmp("사", han) > 0) cho_index = 6;
		else if (strcmp("아", han) > 0) cho_index = 7;
		else if (strcmp("자", han) > 0) cho_index = 8;
		else if (strcmp("차", han) > 0) cho_index = 9;
		else if (strcmp("카", han) > 0) cho_index = 10;
		else if (strcmp("타", han) > 0) cho_index = 11;
		else if (strcmp("파", han) > 0) cho_index = 12;
		else if (strcmp("하", han) > 0) cho_index = 13;
		else cho_index = 14;

		cho[j++] = cho_list[cho_index][0];
		cho[j++] = cho_list[cho_index][1];
		cnt++;
	}
	cho[j] = 0;

}

void CServerDlg::OnClickedBtnChosung2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	srand(time(NULL));
	char* cho_list[] = { "ㄱ", "ㄴ", "ㄷ", "ㄹ", "ㅁ","ㅂ", "ㅅ", "ㅇ", "ㅈ", "ㅊ", "ㅋ","ㅌ", "ㅍ", "ㅎ" };
	char data[256];


	int ra = rand() % 14;
	int ra1 = rand() % 14;
	int ra2 = rand() % 14;

	char c2[5];
	char c3[5];

	strcpy_s(c1, cho_list[ra]);//c1에 cho_list의 초성 중 하나 랜덤으로 복사
	strcpy_s(c2, cho_list[ra1]);
	strcpy_s(c3, cho_list[ra2]);

	strcat_s(c1, c2);//c1 문자열 뒤에 c2 문자열 연결
	strcat_s(c1, c3);

	racho = c1;

	m_ctlchosung.SetWindowText(racho);//static text에 초성 출력

	//클라이언트 리스트 박스에도 초성 출력
	strcpy_s(data, "사회자 : " + racho);
	for each (int i in m_using)
	{
		m_ComSock[i]->Send(data, 256);
	}

	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, racho);
}


void CServerDlg::OnBnClickedTimerStr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	char data[256] = {  "참여자 0 시작"  };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data);
	m_ComSock[0]->Send(data, 256);

}


void CServerDlg::OnBnClickedTimerEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data[256] = { "end 0" };
	end1 = data;
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data);
	m_ComSock[0]->Send(data, 256);

}


void CServerDlg::RandomNum()
{
	unsigned int ranNum[4] = {0, 1, 2, 3 };
	srand((unsigned)time(NULL));
	char data[256];
	CString strNum;
	int start_number = 1, end_number = 4;
	for (int cnt = 0; cnt < 4; cnt++)
	{
		ranNum[cnt] = (rand() % 4)+ 1;
		for (int j = 0; j < cnt; j++)
		{
			if (ranNum[cnt] == ranNum[j])
			{
				cnt--;
				break;
			}
		}
	}
	
	for (int cnt = 0; cnt < 4; cnt++)
	{
		strNum.Format(_T("%d"), ranNum[cnt]);
		strTotal.Append(strNum);
	}
	int i = m_list.GetCount();
	m_list.InsertString(i, strTotal);

	strcpy_s(data, strTotal);
	//4개의 번호를 리스트박스에 출력
	//클라이언트에 데이터 전송
	for each (int i in m_using)
	{
		m_ComSock[i]->Send(data, 256);
	}
}


bool CServerDlg::Repeated(CString str, CString str1)
{
	// TODO: 여기에 구현 코드 추가.
	if (str1.Compare(str) == 0)
	{
		return FALSE;
	}
	return TRUE;
}


void CServerDlg::OnBnClickedTimerStr1()
{
	char data1[256] = { "참여자 1 시작" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data1);
	m_ComSock[1]->Send(data1, 256);

}


void CServerDlg::OnBnClickedTimerStr2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data2[256] = { "참여자 2 시작" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data2);
	m_ComSock[2]->Send(data2, 256);
	
}


void CServerDlg::OnBnClickedTimerStr3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data3[256] = { "참여자 3 시작" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data3);
	m_ComSock[3]->Send(data3, 256);

}


void CServerDlg::OnBnClickedTimerEnd1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data1[256] = { "end 1" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data1);
	m_ComSock[1]->Send(data1, 256);

}


void CServerDlg::OnBnClickedTimerEnd2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data2[256] = { "end 2" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data2);
	m_ComSock[2]->Send(data2, 256);

}


void CServerDlg::OnBnClickedTimerEnd3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char data3[256] = { "end 3" };
	int i = m_chat_list.GetCount();
	m_chat_list.InsertString(i, data3);
	m_ComSock[3]->Send(data3, 256);

}
