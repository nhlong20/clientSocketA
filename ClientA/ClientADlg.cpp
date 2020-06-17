
// ClientADlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ClientA.h"
#include "ClientADlg.h"
#include "afxdialogex.h"
#include "PrivateChat.h"
#include "ChatOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//using namespace Message;

// CClientADlg dialog

CClientADlg::CClientADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTA_DIALOG, pParent)
	, serverIP(_T("127.0.0.1"))
	, m_username(_T("hoanglong"))
	, m_msgString(_T(""))
	, m_password(_T("123456"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, serverIP);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_PASSWORD, c_password);
}

BEGIN_MESSAGE_MAP(CClientADlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CClientADlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_REGISTER, &CClientADlg::OnBnClickedRegister)
END_MESSAGE_MAP()


// CClientADlg message handlers

BOOL CClientADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CClientADlg::initWinSock() {
	// Initialize winsock
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int retcode;
	retcode = WSAStartup(wVersionRequested, &wsaData);
	if (retcode != 0)
		return false;
	return true;
}
bool CClientADlg::createSocket() {
	// Create a socket
	_cSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_cSocket == INVALID_SOCKET)
		return false;
	return true;
}
bool CClientADlg::connectToServer() {
	// Convert CString to string (IP -> sIP)
	CT2CA pszConvertedAnsiString(serverIP);
	std::string sIP(pszConvertedAnsiString);

	// Resolve hostname
	struct hostent* host = NULL;
	if ((host = gethostbyname(sIP.c_str())) == NULL)
		return false;

	// Copy network address to sockaddr_in structure
	memcpy(&srvAddr.sin_addr, host->h_addr_list[0], host->h_length);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, sIP.c_str(), &srvAddr.sin_addr);

	int retcode = connect(_cSocket, (struct sockaddr*)&srvAddr, sizeof(srvAddr));
	if (retcode == SOCKET_ERROR)
		return false;
	_isConnected = true;
	return true;
}

char* CClientADlg::convertToPChar(const CString& cStr)
{
	/*int nSize = s.GetLength();
	char* pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;*/
	// Convert CString to char*
	CT2A str(cStr);
	return str.m_psz;
}

void CClientADlg::OnBnClickedBtnLogin()
{
	UpdateData(TRUE);
	if (m_username == "")
	{
		MessageBox(_T("Username must not be empty!"));
		return;
	}

	if (!this->initWinSock()) {
		MessageBox(L"WSAStartup failed");
		return;
	};

	if (!this->createSocket()) {
		MessageBox(L"Create a Listening socket failed");
		return;
	};

	if (!this->connectToServer()) {
		MessageBox(_T("Cannot connect to server, please try again!"), _T("ERROR"), 0);
		return;
	}
	if (WSAAsyncSelect(_cSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};

	data.type = Messages[Type::LOGIN];
	UpdateData(FALSE);
}
void CClientADlg::OnBnClickedRegister()
{
	UpdateData(TRUE);
	if (m_username == "")
	{
		MessageBox(_T("Username must not be empty!"));
		return;
	}

	if (!this->initWinSock()) {
		MessageBox(L"WSAStartup failed");
		return;
	};

	if (!this->createSocket()) {
		MessageBox(L"Create a Listening socket failed");
		return;
	};
	if (!this->connectToServer()) {
		MessageBox(_T("Cannot connect to server, please try again!"), _T("ERROR"), 0);
		return;
	}
	if (WSAAsyncSelect(_cSocket, m_hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE)) {
		MessageBox(L"Cannot call WSAAsyncSelect");
		return;
	};
	data.type = Messages[Type::REGISTER];
}

LRESULT CClientADlg::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		CommonData response;
		response = receiveCommonData(_cSocket);
		if (response.type == Messages[Type::LOGIN_SUCCESS]) {
			MessageBox(CString(response.message.c_str()), _T("Server response"));
			EndDialog(IDD_CLIENTA_DIALOG);
			bool loginStatus = false;
			ChatOption* chatOption = new ChatOption(this);
			if (chatOption->DoModal() != IDOK) {
				break;
			}
			if (chatOption->chatMode == CHAT_MODE::GROUP_CHAT) {

			}
			if (chatOption->chatMode == CHAT_MODE::PRIVATE_CHAT) {
				 chatOption->m_fUsername;
			}
				
		}
		if (response.type == Messages[Type::LOGIN_FAIL]) {
			MessageBox(CString(response.message.c_str()), _T("Server response"));
		}
		if (response.type == Messages[Type::REG_SUCCESS]) {
			MessageBox(CString(response.message.c_str()), _T("Server response"));
		}
		if (response.type == Messages[Type::REG_FAIL]) {
			MessageBox(CString(response.message.c_str()), _T("Server response"));
		}
		break;
	}
	case FD_WRITE:
	{
		CT2CA uname(this->getUsername());
		CT2CA pwd(this->getPassword());
		std::string username(uname);
		std::string password(pwd);
		if (data.type == Messages[Type::REGISTER]) {
			data.from = "";
			data.to = "";
			data.message = username + "\n" + password + '\0';
			sendCommonData(_cSocket, data);
		}
		if (data.type == Messages[Type::LOGIN]) {
			data.from = "";
			data.to = "";
			data.message = username + "\n" + password + '\0';
			sendCommonData(_cSocket, data);
		}

		UpdateData(FALSE);
		break;
	}
	case FD_CLOSE:
	{
		closesocket(_cSocket);
		MessageBox(_T("Server closed connection"));
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}
	}
	return 0;
}



