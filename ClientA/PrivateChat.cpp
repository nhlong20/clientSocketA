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
	ON_MESSAGE(WM_TEST,SockMsg)
	ON_BN_CLICKED(IDC_SEND, &PrivateChat::OnBnClickedSend)
END_MESSAGE_MAP()


// PrivateChat message handlers


void PrivateChat::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	dataSend.type = Messages[Type::PRIVATE_MSG];

	if (WSAAsyncSelect(m_user->userLogInfo->_cSocket, m_hWnd, WM_TEST, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
}

LRESULT PrivateChat::SockMsg(WPARAM wParam, LPARAM lParam) {
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}	
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ: //receiv 
	{
		UpdateData(TRUE);
		CommonData response;
		response = receiveCommonData(m_user->userLogInfo->_cSocket);
		if (response.type == Messages[Type::TRANFER_MSG_SUC]) {
			MessageBox(L"Aloha");
			std::string msgStr = response.from + ": " + response.message + "\r\n";
			CString msgDisplay = CString(msgStr.c_str());
			c_chatLog.ReplaceSel(msgDisplay);
		}
		else if (response.type == Messages[Type::TRANFER_MSG_FAIL]) {
			MessageBox(L"fail to TRAnfer message");
		}
		UpdateData(FALSE);
		break;
	}
	case FD_WRITE: //send
	{
		UpdateData(TRUE);
		if (dataSend.type == Messages[Type::PRIVATE_MSG]) {
			// Convert message from CString to string
			CT2CA msgBuffer(m_msgChat);
			dataSend.from = convertCStrToStr(m_user->userLogInfo->getUsername());
			dataSend.to = convertCStrToStr(m_user->m_fUsername);
			dataSend.message = std::string(msgBuffer);
			sendCommonData(m_user->userLogInfo->_cSocket, dataSend);
			m_msgChat = "";
		}
		UpdateData(FALSE);
		break;
	}
	case FD_CLOSE:
	{

		closesocket(m_user->userLogInfo->_cSocket);
		MessageBox(_T("Server closed connection"));
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}
	}
	return 0;
}
