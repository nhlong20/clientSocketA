#pragma once

#include "ChatOption.h"
// GroupChat dialog

class GroupChat : public CDialogEx
{
	DECLARE_DYNAMIC(GroupChat)

public:
	GroupChat(ChatOption* doc,CWnd* pParent = nullptr);   // standard constructor
	virtual ~GroupChat();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPCHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ChatOption* m_user;
	CString m_chatBox;
};
