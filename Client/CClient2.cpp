// CClient2.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "CClient2.h"
#include "afxdialogex.h"
#include "CLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClient2 대화 상자

IMPLEMENT_DYNAMIC(CClient2, CDialogEx)

CClient2::CClient2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_strUserID(_T(""))
	, m_strIP(_T("127.0.0.1"))
	, m_strSendText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CClient2::~CClient2()
{
}

void CClient2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_LIST, m_chat_list1);
	DDX_Control(pDX, IDC_CLIENT_SEQ, m_list1);
	DDX_Text(pDX, IDC_EDIT_USERID, m_strUserID);
	DDX_Text(pDX, IDC_IP_ADDR, m_strIP);
	DDX_Text(pDX, IDC_SEND_TEXT, m_strSendText);
	DDX_Control(pDX, IDC_TIMER, m_progress1);
}


BEGIN_MESSAGE_MAP(CClient2, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_SEND, &CClient2::OnSend1)
	ON_BN_CLICKED(IDOK, &CClient2::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CClient2::OnClickedButton1)
	ON_BN_CLICKED(IDC_CONNECT, &CClient2::OnConnect1)
	
END_MESSAGE_MAP()

BOOL CClient2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFF0) == IDM_ABOUTBOX);
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

	return TRUE;
}

// 시스템 명령 처리 메시지 함수, 프로그램 종료 시 서버에 메시지를 보냄
void CClient2::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*/if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}*/
	if (nID == SC_CLOSE)
	{
		//클라이언트 종료 시 서버로 메시지 보냄
		m_ComSock2.Send(SOC_CLIENT_DISCONNECT, 256);
		this->EndDialog(IDCANCEL);

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CClient2::OnPaint()
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

HCURSOR CClient2::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LPARAM CClient2::OnReceive(UINT wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	char data[256];
	CString str;
	memset(data, '\0', 256);

	//데이터를 data에 받음
	m_ComSock2.Receive(data, 256);
	str.Format(_T("%s"), data);// int형을 문자열로 변환

	//서버로부터 연결완료 메시지 받으면
	if (str.Find(S0C_CLIENT_CONNECT) == 0)
	{
		//Right 함수 사용해 가장 오른쪽에 있는 번호 추출, 0은 \0임
		m_strUserID = "참여자: " + str.Right(1);
		
	}
	else
	{
		int index = 0;//find 함수에 대한 return 값
		index = str.Find("4");//서버에서 받은 str에 문자열 4가 포함되어있는지 확인
							  //4를 포함하고 있지않으면 -1 반환

		//리스트 박스에 보여줌
		if (index != -1)//반환값이 -1이 아니면 4를 포함하고 있으므로
		{
			int j = m_list1.GetCount();
			m_list1.InsertString(j, str);
		}
		else
		{
			if (str == "참여자 0 시작")
			{
				m_progress1.OffsetPos(-10);//offsetpos : 초기화 시키는 것

				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				if (0 >= m_timer_sec1) m_timer_sec1 = 7;

				// 입력된 타이머 시간(초)로 PGC의 Range를 재설정
				m_progress1.SetRange(0, m_timer_sec1);

				//설정된 타이머 시간으로 PGC Bar를 위치시킨다.
				m_progress1.SetPos(m_timer_sec1);

				//1초 주기로 WM_TIMER 메세지를 발생하기 위한 타이머 생성
				
				SetTimer(1, 1000, NULL);
				
			}
			else if (str == "참여자 1 시작")
			{
				m_progress1.OffsetPos(-10);//offsetpos : 초기화 시키는 것

				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				if (0 >= m_timer_sec1) m_timer_sec1 = 5;

				// 입력된 타이머 시간(초)로 PGC의 Range를 재설정
				m_progress1.SetRange(0, m_timer_sec1);

				//설정된 타이머 시간으로 PGC Bar를 위치시킨다.
				m_progress1.SetPos(m_timer_sec1);

				//1초 주기로 WM_TIMER 메세지를 발생하기 위한 타이머 생성

				SetTimer(2, 1000, NULL);

			}
			else if(str == "참여자 2 시작")
			{
				m_progress1.OffsetPos(-10);//offsetpos : 초기화 시키는 것

				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				if (0 >= m_timer_sec1) m_timer_sec1 = 5;

				// 입력된 타이머 시간(초)로 PGC의 Range를 재설정
				m_progress1.SetRange(0, m_timer_sec1);

				//설정된 타이머 시간으로 PGC Bar를 위치시킨다.
				m_progress1.SetPos(m_timer_sec1);

				//1초 주기로 WM_TIMER 메세지를 발생하기 위한 타이머 생성

				SetTimer(3, 1000, NULL);

			}
			else if (str == "참여자 3 시작")
			{
				m_progress1.OffsetPos(-10);//offsetpos : 초기화 시키는 것

				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				if (0 >= m_timer_sec1) m_timer_sec1 = 5;

				// 입력된 타이머 시간(초)로 PGC의 Range를 재설정
				m_progress1.SetRange(0, m_timer_sec1);

				//설정된 타이머 시간으로 PGC Bar를 위치시킨다.
				m_progress1.SetPos(m_timer_sec1);

				//1초 주기로 WM_TIMER 메세지를 발생하기 위한 타이머 생성

				SetTimer(4, 1000, NULL);

			}
			else if (str == "end 0")
			{
				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				m_progress1.SetRange(0, m_timer_sec1);
				m_progress1.SetPos(m_timer_sec1);
				SetTimer(1, 1000, NULL);
				if (m_timer_sec1 > 0)
				{
					ResetTimer1();
					return 0;
				}
			}
			else if (str == "end 1")
			{
				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				m_progress1.SetRange(0, m_timer_sec1);
				m_progress1.SetPos(m_timer_sec1);
				SetTimer(2, 1000, NULL);
				if (m_timer_sec1 > 0)
				{
					ResetTimer2();
					return 0;
				}

			}
			else if (str == "end 2")
			{
				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				m_progress1.SetRange(0, m_timer_sec1);
				m_progress1.SetPos(m_timer_sec1);
				SetTimer(3, 1000, NULL);
				if (m_timer_sec1 > 0)
				{
					ResetTimer3();
					return 0;
				}

			}
			else if (str == "end 3")
			{
				m_timer_sec1 = GetDlgItemInt(IDC_TIMER);
				m_progress1.SetRange(0, m_timer_sec1);
				m_progress1.SetPos(m_timer_sec1);
				SetTimer(4, 1000, NULL);
				if (m_timer_sec1 > 0)
				{
					ResetTimer4();
					return 0;
				}

			}
			int i = m_chat_list1.GetCount();//listbox의 데이터 갯수
			m_chat_list1.InsertString(i, str);
		}


	}
	UpdateData(FALSE);// 컨트롤에 멤버 변수의 내용을 표시
	return TRUE;
}

// CClient2 메시지 처리기


void CClient2::OnSend1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);//컨트롤 내용을 다이얼로그 클래스의 멤버변수로 저장
	char data[256];
	CString str;

	//data에 전송할 데이터 입력
	memset(data, '\0', 256);
	strcpy_s(data, m_strSendText);
	m_strSendText = "";

	//전송
	m_ComSock2.Send(data, 256);

	//전송한 데이터도 리스트박스에 보여줌
	str.Format(_T("%s"), data);
	int i = m_chat_list1.GetCount();
	m_chat_list1.InsertString(i, str);

	UpdateData(FALSE);//컨트롤에 멤버 변수의 내용을 표시
}


void CClient2::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ComSock2.Send(SOC_CLIENT_DISCONNECT, 256);//서버로 종료 메시지 보냄
	this->EndDialog(IDCANCEL); //Dialog 닫기
	CDialogEx::OnOK();
}


void CClient2::OnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox(_T("1. 명사만 가능 \n 2. 사람 이름 X \n 3. 중복 X \n 4. 5초 안에 입력"));//규칙
}

void CClient2::OnConnect1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ComSock2.Create();
	// IP 주소와 포트 번호를 지정
	m_ComSock2.Connect(m_strIP, 7777);
	m_ComSock2.Init(this->m_hWnd);
	UpdateData(FALSE);
	
}


void CClient2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 구현 코드 추가.

	switch (nIDEvent)
	{
	case 1:
		m_progress1.OffsetPos(-1);
		if (m_progress1.GetPos() == 0)
		{
			m_ComSock2.Send("1", 256);
			ResetTimer1();
		}
		break;

	case 2:
		m_progress1.OffsetPos(-1);
		if (m_progress1.GetPos() == 0)
		{
			m_ComSock2.Send("2", 256);
			ResetTimer2();
		}
		break;
	case 3:
		m_progress1.OffsetPos(-1);
		if (m_progress1.GetPos() == 0)
		{
			m_ComSock2.Send("3", 256);
			ResetTimer3();
		}
		break;
	case 4:
		m_progress1.OffsetPos(-1);
		if (m_progress1.GetPos() == 0)
		{
			m_ComSock2.Send("4", 256);
			ResetTimer4();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CClient2::ResetTimer1()
{
	// TODO: 여기에 구현 코드 추가.
	KillTimer(1);

	m_progress1.SetRange(0, 100);

	m_progress1.SetPos(0);

	m_timer_sec1 = 0;
}

void CClient2::ResetTimer2()
{
	// TODO: 여기에 구현 코드 추가.
	KillTimer(2);

	m_progress1.SetRange(0, 100);

	m_progress1.SetPos(0);

	m_timer_sec1 = 0;
}


void CClient2::ResetTimer3()
{
	// TODO: 여기에 구현 코드 추가.
	KillTimer(3);

	m_progress1.SetRange(0, 100);

	m_progress1.SetPos(0);

	m_timer_sec1 = 0;
}


void CClient2::ResetTimer4()
{
	// TODO: 여기에 구현 코드 추가.
	KillTimer(4);

	m_progress1.SetRange(0, 100);

	m_progress1.SetPos(0);

	m_timer_sec1 = 0;
}
