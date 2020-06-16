#pragma once
#include "ClientADlg.h"

// ChatOption dialog

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
	CClientADlg* userInfo;
	int chatOption;
	CEdit c_fUsername;
	CString m_fUsername;
};
