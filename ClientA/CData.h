#pragma once
#include <string>
#define DEFAULT_BUFFER 4096
enum Type {
	LOGIN,
	LOGIN_SUCCESS,
	LOGIN_FAIL,
	REGISTER,
	REG_SUCCESS,
	REG_FAIL,
	LOGOUT,
	LOGOUT_SUC,
	USER_EXIST,
	USER_NOT_EXIST,
	PRIVATE_CHAT_TO,
	UPLOAD_PFILES,
	SHOW_PLFILES,
	DOWN_PFILES,
	CHAT_READY,
	CJ_GROUP_CHAT,
	PRIVATE_MSG,
	TRANFER_MSG_SUC,
	TRANFER_MSG_FAIL,
};

static std::string Messages[] = {
"LOGIN",
"LOGIN_SUCCESS",
"LOGIN_FAIL",
"REGISTER",
"REG_SUCCESS",
"REG_FAIL",
"LOGOUT",
"LOGOUT_SUC",
"USER_EXIST",
"USER_NOT_EXIST"
"PRIVATE_CHAT_TO",
"UPLOAD_PFILES",
"SHOW_PLFILES",
"DOWN_PFILES",
"CHAT_READY",
"CJ_GROUP_CHAT",
"PRIVATE_MSG",
"TRANFER_MSG_SUC",
"TRANFER_MSG_FAIL",
};

struct CommonData {
	std::string from;	// senderUsername
	std::string to;		// receiverUsername
	std::string type;   // MessagesType
	std::string message;
};
std::string convertCStrToStr(CString cStr);
void sendCommonData(SOCKET& sock, CommonData& data);
CommonData receiveCommonData(SOCKET& sock);