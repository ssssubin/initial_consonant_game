#pragma once
#include <afxsock.h> //���� Ŭ���� ��� ����
#include "CSocServer.h"//������ ����
#include "CSocCom.h"//��ſ� ����

#define UM_ACCEPT WM_USER+1 // ���� �����쿡 �޽����� ������ ���� ������
// ���� �����쿡 SendMessage�� �̿��� UM_ACCEPT ��� ����� ���� �޽����� ��
#define UM_RECEIVE WM_USER+2// UM_ACCEPT�� ����� ����