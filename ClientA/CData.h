#pragma once
#include <string>
#define DEFAULT_BUFLEN 512
enum Type {
	LOGIN,
	LOGIN_SUCCESS,
	LOGIN_FAIL,
	REGISTER,
	REG_SUCCESS,
	REG_FAIL,
	LOGOUT,
	USER_EXIST,
	USER_NOT_EXIST,
	PRIVATE_CHAT_TO,
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
"USER_EXIST",
"USER_NOT_EXIST"
"PRIVATE_CHAT_TO",
"PRIVATE_MSG",
"TRANFER_MSG_SUC",
"TRANFER_MSG_FAIL",
};

struct CommonData {
	std::string from;	// senderIP
	std::string to;		// receiverIP
	std::string type;   // MessagesType
	std::string message;
	//CommonTime timeStampt;
	//int fileSize;	// from 1 to 5 * 1024 * 1024 Bytes
};
std::string convertCStrToStr(CString cStr);
void sendCommonData(SOCKET& sock, CommonData& data);
CommonData receiveCommonData(SOCKET& sock);