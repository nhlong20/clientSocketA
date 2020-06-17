// PrivateChat.cpp : implementation file
//

#include "pch.h"
#include "ClientA.h"
#include "PrivateChat.h"
#include "afxdialogex.h"


// PrivateChat dialog

IMPLEMENT_DYNAMIC(PrivateChat, CDialogEx)

PrivateChat::PrivateChat(ChatOption* doc,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRIVATECHAT, pParent)
	, m_msgChat(_T(""))
{
	m_user = doc;
}

PrivateChat::~PrivateChat()
{
}

void PrivateChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHATBOX, m_msgChat);
	DDX_Control(pDX, IDC_CHATBOX, c_msgChat);
	DDX_Control(pDX, IDC_CHATLOG, c_chatLog);
	GetDlgItem(IDS_RECEIVER)->SetWindowText(m_user->m_fUsername);
}


BEGIN_MESSAGE_MAP(PrivateChat, CDialogEx)
	ON_BN_CLICKED(IDC_SEND, &PrivateChat::OnBnClickedSend)
END_MESSAGE_MAP()


// PrivateChat message handlers


void PrivateChat::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	/*c_msgChat.GetWindowText(m_msgChat);*/
	m_msgChat = m_user->userLogInfo->getUsername() + ": " + m_msgChat;
	m_msgChat += "\r\n";
	c_chatLog.ReplaceSel(m_msgChat);
	m_msgChat = "";
	UpdateData(FALSE);
}
