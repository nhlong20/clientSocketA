#pragma once
#include "ChatOption.h"
// PrivateChat dialog
#define WM_SOCKET WM_USER+2
class PrivateChat : public CDialogEx
{
	DECLARE_DYNAMIC(PrivateChat)

public:
	PrivateChat(ChatOption* doc = nullptr,CWnd* pParent = nullptr);   // standard constructor
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
	void displayMsg(const CommonData&);
	
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	CString m_msgChat;
	CEdit c_msgChat;
	CEdit c_chatLog;
	CommonData dataSend;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedAttach();
	CListCtrl m_files;
	afx_msg void OnBnClickedUpload();
	void sendFile(CommonData data);
	void receiveFile(CommonData response);
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedActivate();
	CMFCButton c_activeBtn;
};
