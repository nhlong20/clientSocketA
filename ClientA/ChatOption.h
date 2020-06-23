#pragma once
#include "ClientADlg.h"

// ChatOption dialog
#define WM_PRIVATECHAT WM_USER+3
enum CHAT_MODE { PRIVATE_CHAT, GROUP_CHAT };
class ChatOption : public CDialogEx
{
	DECLARE_DYNAMIC(ChatOption)

public:
	ChatOption(CClientADlg* doc,CWnd* pParent = nullptr);   // standard constructor
	virtual ~ChatOption();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CClientADlg* userLogInfo;
	int chatMode;
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	CEdit c_fUsername;
	CString m_fUsername;

	//PrivateChat privateChatDlg;
	afx_msg void OnBnClickedPrivateChat();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedGroupchat();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	CString m_groupID;

};
