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
	PRIVATE_CHAT_TO,
	USER_EXIST,
	USER_NOT_EXIST,
};

static std::string Messages[] = {
"LOGIN",
"LOGIN_SUCCESS",
"LOGIN_FAIL",
"REGISTER",
"REG_SUCCESS",
"REG_FAIL",
"PRIVATE_CHAT_TO",
"USER_EXIST",
"USER_NOT_EXIST"
};

struct CommonData {
	std::string from;	// senderIP
	std::string to;		// receiverIP
	std::string type;   // MessagesType
	std::string message;
	//CommonTime timeStampt;
	//int fileSize;	// from 1 to 5 * 1024 * 1024 Bytes
};
void convertStr2WStr(std::string& str, std::wstring& wstr);
void sendCommonData(SOCKET& sock, CommonData& data);
CommonData receiveCommonData(SOCKET& sock);