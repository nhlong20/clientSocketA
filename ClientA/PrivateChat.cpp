// PrivateChat.cpp : implementation file
//

#include "pch.h"
#include "ClientA.h"
#include "PrivateChat.h"
#include "afxdialogex.h"


// PrivateChat dialog

IMPLEMENT_DYNAMIC(PrivateChat, CDialogEx)

PrivateChat::PrivateChat(ChatOption* doc, CWnd* pParent /*=nullptr*/)
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
	GetDlgItem(IDS_RECEIVER)->SetWindowText(L"To: " + m_user->m_fUsername);
	DDX_Control(pDX, IDC_FILES, m_files);
	DDX_Control(pDX, IDC_ACTIVATE, c_activeBtn);
}


BEGIN_MESSAGE_MAP(PrivateChat, CDialogEx)
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_SEND, &PrivateChat::OnBnClickedSend)
	ON_BN_CLICKED(IDC_UPLOAD, &PrivateChat::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_DOWNLOAD, &PrivateChat::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_ACTIVATE, &PrivateChat::OnBnClickedActivate)
END_MESSAGE_MAP()


// PrivateChat message handlers

void PrivateChat::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dataSend.type = Messages[Type::PRIVATE_MSG];
	if (dataSend.type == Messages[Type::PRIVATE_MSG]) {
		// Convert message from CString to string
		CT2CA msgBuffer(m_msgChat);
		dataSend.from = convertCStrToStr(m_user->userLogInfo->getUsername());
		dataSend.to = convertCStrToStr(m_user->m_fUsername);
		dataSend.message = std::string(msgBuffer);
		sendCommonData(m_user->userLogInfo->_cSocket, dataSend);
		m_msgChat = "";
	}/*
	UpdateData(FALSE);*/

	if (WSAAsyncSelect(m_user->userLogInfo->_cSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
	UpdateData(FALSE);
}



void PrivateChat::displayMsg(const CommonData& res) {
	std::string msgStr;
	if (res.type == Messages[Type::SHOW_PLFILES]) {
		std::string fileName = res.message.substr(res.message.find_last_of("/\\") + 1);
		msgStr = res.from + " uploaded a file name: " + res.message + "\r\n";
	}
	else if (res.type == Messages[Type::LOGOUT_SUC]) {
		msgStr = res.message;
	}
	else {
		msgStr = res.from + ": " + res.message + "\r\n";
	}
	CString msgDisplay = CString(msgStr.c_str());
	c_msgChat.GetWindowText(m_msgChat);
	// get the initial text length
	int nLength = c_chatLog.GetWindowTextLength();
	// put the selection at the end of text
	c_chatLog.SetSel(nLength, nLength);
	// replace the selection
	c_chatLog.ReplaceSel(msgDisplay);
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
			displayMsg(response);
		}
		else if (response.type == Messages[Type::TRANFER_MSG_FAIL]) {
			MessageBox(L"Fail to tranfer message");
		}
		if (response.type == Messages[Type::SHOW_PLFILES]) {
			std::string fileNameStr = response.message.substr(response.message.find_last_of("/\\") + 1);
			CString fileName = CString(fileNameStr.c_str());
			m_files.InsertItem(0, fileName);
		}
		if (response.type == Messages[Type::LOGOUT_SUC]) {
			displayMsg(response);
		}
		UpdateData(FALSE);
		break;
	}
	case FD_WRITE: //send
	{
		UpdateData(TRUE);

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

void PrivateChat::OnBnClickedUpload()
{
	CFileDialog fileDlg(TRUE);
	CommonData fileInfo;
	CString filePath;
	if (fileDlg.DoModal() == IDOK) {
		filePath = fileDlg.GetPathName();
		fileInfo.type = Messages[Type::UPLOAD_PFILES];
		fileInfo.message = convertCStrToStr(filePath);
		fileInfo.from = convertCStrToStr(m_user->userLogInfo->getUsername());
		fileInfo.to = convertCStrToStr(m_user->m_fUsername);
		sendFile(fileInfo);
		m_files.InsertItem(0, fileDlg.GetFileName());
	}
}
void PrivateChat::sendFile(CommonData fileData) {
	SOCKET _cFileSocket;
	if (!CClientADlg::initWinSock()) {
		MessageBox(L"WSAStartup failed", _T("ERROR"), 0);
		return;
	};
	if (!CClientADlg::createSocket(_cFileSocket)) {
		MessageBox(L"Create a Listening socket failed", _T("ERROR"), 0);
		return;
	};
	if (!CClientADlg::connectToServer(_cFileSocket, "127.0.0.1", PORT)) {
		MessageBox(_T("Cannot connect to server, please try again!"), _T("ERROR"), 0);
		return;
	}

	sendCommonData(_cFileSocket, fileData);
	std::string filePath = fileData.message;
	FILE* fi = fopen(filePath.c_str(), "rb");
	if (!fi) {
		MessageBox(L"Error, File not found!", _T("ERROR"), 0);
		return;
	}
	int fileSize, bytes_to_send, bytes_sent;
	byte* buffer = NULL;
	int SEND_BUFFER_SIZE = 4096;
	// Lay kich thuoc file
	fseek(fi, 0, SEEK_END);
	fileSize = ftell(fi);
	fseek(fi, 0, SEEK_SET);

	// Gui kich thuoc file

	bytes_to_send = sizeof(fileSize);
	do
	{
		int size = fileSize + sizeof(fileSize) - bytes_to_send;
		bytes_sent = send(_cFileSocket, (char*)&size, bytes_to_send, 0);

		if (bytes_sent == SOCKET_ERROR)
			return;

		bytes_to_send -= bytes_sent;
	} while (bytes_to_send > 0);


	// Gui file's data 
	buffer = new byte[SEND_BUFFER_SIZE];
	bytes_to_send = fileSize;// So bytes data can gui

	do {
		int buffer_size;
		buffer_size = (SEND_BUFFER_SIZE > bytes_to_send) ? bytes_to_send : SEND_BUFFER_SIZE;
		fread(buffer, buffer_size, 1, fi);

		// Neu gui bi loi thi gui lai goi tin do
		do {
			bytes_sent = send(_cFileSocket, (char*)buffer, buffer_size, 0);
		} while (bytes_sent == -1);

		// Cap nhat lai so bytes can gui
		bytes_to_send -= bytes_sent;
	} while (bytes_to_send > 0);

	if (buffer) delete[] buffer;
	fclose(fi);
	MessageBox(L"Uploaded successfully");
	if (WSAAsyncSelect(_cFileSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};

}
void PrivateChat::receiveFile(CommonData fileData) {
	SOCKET _cFileSocket;
	if (!CClientADlg::initWinSock()) {
		MessageBox(L"WSAStartup failed", _T("ERROR"), 0);
		return;
	};
	if (!CClientADlg::createSocket(_cFileSocket)) {
		MessageBox(L"Create a Listening socket failed", _T("ERROR"), 0);
		return;
	};
	if (!CClientADlg::connectToServer(_cFileSocket, "127.0.0.1", PORT)) {
		MessageBox(_T("Cannot connect to server, please try again!"), _T("ERROR"), 0);
		return;
	}
	sendCommonData(_cFileSocket, fileData);
	std::string fileName = fileData.message;
	// Receiver file
	int fileSize = 0, bytes_recevived, bytes_to_receive;
	byte* buffer = NULL;
	// Mo file
	FILE* fo = fopen(fileName.c_str(), "wb");

	// Nhan kich thuoc file
	int size = 0;
	bytes_to_receive = sizeof(fileSize);
	do
	{
		size = (fileSize)+sizeof(fileSize) - bytes_to_receive;
		bytes_recevived = recv(_cFileSocket, (char*)&size, bytes_to_receive, 0);

		if (bytes_recevived == SOCKET_ERROR || bytes_recevived == 0)
			return;

		bytes_to_receive -= bytes_recevived;
	} while (bytes_to_receive > 0);

	// Nhan file's data
	buffer = new byte[DEFAULT_BUFFER];
	bytes_to_receive = size;

	do {
		int buffer_size;
		buffer_size = (DEFAULT_BUFFER > bytes_to_receive) ? bytes_to_receive : DEFAULT_BUFFER;

		do {
			bytes_recevived = recv(_cFileSocket, (char*)buffer, buffer_size, 0);
		} while (bytes_recevived == SOCKET_ERROR);

		fwrite((char*)buffer, bytes_recevived, 1, fo);

		memset(buffer, 0, DEFAULT_BUFFER);
		bytes_to_receive -= bytes_recevived;
	} while (bytes_to_receive > 0);

	if (buffer) delete[] buffer;
	fclose(fo);

	MessageBox(L"Downloaded successfully");
	if (WSAAsyncSelect(_cFileSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
}

void PrivateChat::OnBnClickedDownload()
{
	for (int i = 0; i < m_files.GetItemCount(); i++)
	{
		if (m_files.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			CString fileName = m_files.GetItemText(i, 0);
			CString cstr = _T(" download ");
			CommonData fileInfo;
			fileInfo.type = Messages[Type::DOWN_PFILES];
			fileInfo.message = convertCStrToStr(fileName);
			fileInfo.from = convertCStrToStr(m_user->userLogInfo->getUsername());
			receiveFile(fileInfo);
		}
	}
}


void PrivateChat::OnBnClickedActivate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_ACTIVATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHATBOX)->EnableWindow(TRUE);
	GetDlgItem(IDC_UPLOAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_DOWNLOAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_FILES)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHATLOG)->EnableWindow(TRUE);
	
	dataSend.type = Messages[Type::CHAT_READY];
	sendCommonData(m_user->userLogInfo->_cSocket, dataSend);
	if (WSAAsyncSelect(m_user->userLogInfo->_cSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
	UpdateData(FALSE);
}
