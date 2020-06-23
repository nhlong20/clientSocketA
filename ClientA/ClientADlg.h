#pragma once
#define PORT 25000
#define WM_SOCKET WM_USER+2
#include "CData.h"
// CClientADlg dialog
class CClientADlg : public CDialogEx
{
// Construction
public:
	CClientADlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//---------- Client Controller -----------
public:
	static bool initWinSock();
	static bool createSocket(SOCKET& _socket);
	static bool connectToServer(SOCKET _socket, std::string sIP, int serverPort);

	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	char* convertToPChar(const CString&);
	void Split(CString src, CString des[2]);
	void mSend(CString Command);
	int mRecv(CString& Command);
public:
	SOCKET _cSocket;

	//UINT _serverPort, _fUserPort, _userPort;
	bool _isConnected;

	CString Command;
	int	buffLength;
	CString userOnline;
public:
	CString getUsername() { return m_username; }
	CString getPassword() { return m_password; }
	CommonData data;
	CString serverIP;
private:
	CString m_msgString;
	CEdit c_password;
	CString m_username;
	CString m_password;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedRegister();
};
