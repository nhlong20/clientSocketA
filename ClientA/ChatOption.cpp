// ChatOption.cpp : implementation file
//

#include "pch.h"
#include "ClientA.h"
#include "ChatOption.h"
#include "afxdialogex.h"


// ChatOption dialog

IMPLEMENT_DYNAMIC(ChatOption, CDialogEx)

ChatOption::ChatOption(CClientADlg* doc,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_OPTIONS, pParent)
	, m_fUsername(_T(""))
{
	userInfo = doc;
}

ChatOption::~ChatOption()
{
}

void ChatOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PRIVATE_RADIO, chatOption);
	DDX_Control(pDX, IDC_FRIEND_USERNAME, c_fUsername);
	DDX_Text(pDX, IDC_FRIEND_USERNAME, m_fUsername);
}


BEGIN_MESSAGE_MAP(ChatOption, CDialogEx)
END_MESSAGE_MAP()


// ChatOption message handlers
