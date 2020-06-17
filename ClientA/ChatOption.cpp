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
	, m_fUsername(_T("trannghia"))
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
}


BEGIN_MESSAGE_MAP(ChatOption, CDialogEx)
	ON_BN_CLICKED(IDC_PRIVATE_CHAT, &ChatOption::OnBnClickedPrivateChat)
	ON_BN_CLICKED(IDOK, &ChatOption::OnBnClickedOk)
	ON_MESSAGE(WM_PRIVATECHAT,SockMsg)
	ON_BN_CLICKED(IDC_RADIO2, &ChatOption::OnBnClickedRadio2)
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
	if (WSAAsyncSelect(userLogInfo->_cSocket, m_hWnd, WM_PRIVATECHAT, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
	//CDialogEx::OnOK();
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
			EndDialog(IDD_CHAT_OPTIONS);
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
		if (userLogInfo->data.type == Messages[Type::PRIVATE_CHAT_TO]) {

			CT2CA bufferUsername(userLogInfo->getUsername());
			CT2CA bufferFUsername(m_fUsername);

			std::string username(bufferUsername);
			std::string fUsername(bufferFUsername);

			userLogInfo->data.from = username;
			userLogInfo->data.to = fUsername;
			sendCommonData(userLogInfo->_cSocket, userLogInfo->data);
		}
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
