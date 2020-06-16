#pragma once
#include <string>
#define DEFAULT_BUFLEN 512
struct CommonData {
	std::string from;	//source
	std::string to;		//destination
	std::string type; // 	PCHAT, GCHAT, P_FILE, G_FILE
	std::string message;
	//CommonTime timeStampt;
	//int fileSize;	// from 1 to 5 * 1024 * 1024 Bytes
};
void convertStr2WStr(std::string& str, std::wstring& wstr);
void sendCommonData(SOCKET& sock, CommonData& data);
CommonData receiveCommonData(SOCKET& sock);