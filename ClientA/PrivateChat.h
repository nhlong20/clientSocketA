#pragma once
#include "ChatOption.h"

// PrivateChat dialog
#define WM_TEST WM_USER+4
class PrivateChat : public CDialogEx
{
	DECLARE_DYNAMIC(PrivateChat)

public:
	PrivateChat(ChatOption* doc,CWnd* pParent = nullptr);   // standard constructor
	virtual ~PrivateChat();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIVATECHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	ChatOption* m_user;
private:
	void setSenderReceiver(const CString& sender, const CString& receiver);
	
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	CString m_msgChat;
	CEdit c_msgChat;
	CEdit c_chatLog;
	CommonData dataSend;
	afx_msg void OnBnClickedSend();
};
