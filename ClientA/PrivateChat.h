#pragma once
#include "ClientADlg.h"

// PrivateChat dialog

class PrivateChat : public CDialogEx
{
	DECLARE_DYNAMIC(PrivateChat)

public:
	PrivateChat(CClientADlg* doc,CWnd* pParent = nullptr);   // standard constructor
	virtual ~PrivateChat();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIVATECHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CClientADlg* m_user;
public:
	CString m_msgChat;
	CEdit c_msgChat;
	CEdit c_chatLog;
	afx_msg void OnBnClickedSend();
};