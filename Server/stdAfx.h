#pragma once
#include <afxsock.h> //소켓 클래스 사용 위해
#include "CSocServer.h"//서버용 소켓
#include "CSocCom.h"//통신용 소켓

#define UM_ACCEPT WM_USER+1 // 메인 윈도우에 메시지를 보내기 위한 구별자
// 메인 윈도우에 SendMessage를 이용해 UM_ACCEPT 라는 사용자 정의 메시지를 줌
#define UM_RECEIVE WM_USER+2// UM_ACCEPT와 비슷한 역할