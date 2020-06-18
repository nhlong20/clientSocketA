// ChatOption.cpp : implementation file
//

#include "pch.h"
#include "ClientA.h"
#include "ChatOption.h"
#include "afxdialogex.h"
#include "CData.h"
#include "PrivateChat.h"

// ChatOption dialog

IMPLEMENT_DYNAMIC(ChatOption, CDialogEx)

ChatOption::ChatOption(CClientADlg* doc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_OPTIONS, pParent)
	, m_fUsername(_T("hoanglong"))
{
	userLogInfo = doc;
	chatMode = -1;
}

ChatOption::~ChatOption()
{
}

void ChatOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PRIVATE_CHAT, chatMode);	
	DDX_Control(pDX, IDC_FRIEND_USERNAME, c_fUsername);
	DDX_Text(pDX, IDC_FRIEND_USERNAME, m_fUsername);
	GetDlgItem(IDS_HELLO)->SetWindowText(_T("Hi " + userLogInfo->getUsername() +","));
}


BEGIN_MESSAGE_MAP(ChatOption, CDialogEx)
	ON_BN_CLICKED(IDC_PRIVATE_CHAT, &ChatOption::OnBnClickedPrivateChat)
	ON_BN_CLICKED(IDOK, &ChatOption::OnBnClickedOk)
	ON_MESSAGE(WM_PRIVATECHAT,SockMsg)
	ON_BN_CLICKED(IDC_RADIO2, &ChatOption::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_LOGOUT, &ChatOption::OnBnClickedLogout)
END_MESSAGE_MAP()


// ChatOption message handlers
void ChatOption::OnBnClickedPrivateChat()
{
	chatMode = CHAT_MODE::PRIVATE_CHAT;
	userLogInfo->data.type = Messages[Type::PRIVATE_CHAT_TO];
}

void ChatOption::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (chatMode == -1) {
		MessageBox(L"You must choose chat mode to begin");
		return;
	}
	CT2CA bufferUsername(userLogInfo->getUsername());
	CT2CA bufferFUsername(m_fUsername);

	std::string username(bufferUsername);
	std::string fUsername(bufferFUsername);

	userLogInfo->data.from = username;
	userLogInfo->data.to = fUsername;
	sendCommonData(userLogInfo->_cSocket, userLogInfo->data);

	if (WSAAsyncSelect(userLogInfo->_cSocket, m_hWnd, WM_PRIVATECHAT, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};

}


void ChatOption::OnBnClickedRadio2()
{
	chatMode = CHAT_MODE::GROUP_CHAT;

}
LRESULT ChatOption::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ: //receiv 
	{
		CommonData response;
		response = receiveCommonData(userLogInfo->_cSocket);
	
		if (chatMode == CHAT_MODE::PRIVATE_CHAT && response.type == Messages[Type::USER_EXIST]) {
			PrivateChat* privateChatDlg = new PrivateChat(this);
			privateChatDlg->DoModal();
		}
		else if (chatMode == CHAT_MODE::PRIVATE_CHAT && response.type == Messages[Type::USER_NOT_EXIST]){
			MessageBox(L"User do not exist, try again!");
		}
		break;
	}
	case FD_WRITE: //send
	{
		break;
	}
	case FD_CLOSE:
	{
		closesocket(userLogInfo->_cSocket);
		MessageBox(_T("Server closed connection"));
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}
	}
	return 0;
}


void ChatOption::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Do you wanna logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL) return;
	CommonData request;
	request.type = Messages[Type::LOGOUT];
	request.from = convertCStrToStr(userLogInfo->getUsername());
	request.to = "";
	request.message = "";
	sendCommonData(userLogInfo->_cSocket, request);
	CDialogEx::OnOK();
}
