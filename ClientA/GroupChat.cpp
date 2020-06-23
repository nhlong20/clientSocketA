// GroupChat.cpp : implementation file
//

#include "pch.h"
#include "ClientA.h"
#include "GroupChat.h"
#include "afxdialogex.h"


// GroupChat dialog

IMPLEMENT_DYNAMIC(GroupChat, CDialogEx)

GroupChat::GroupChat(ChatOption*doc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROUPCHAT, pParent)
	, m_chatBox(_T(""))
{
	m_user = doc;
}

GroupChat::~GroupChat()
{
}

void GroupChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHATBOX, m_chatBox);
}


BEGIN_MESSAGE_MAP(GroupChat, CDialogEx)
END_MESSAGE_MAP()


// GroupChat message handlers
