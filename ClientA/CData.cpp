#include "pch.h"
#include "CData.h"
void sendCharArr(SOCKET& sock, const char* charArr) {
	int buffLen = strlen(charArr)*2;
	send(sock, (char*)&buffLen, sizeof(buffLen), 0);
	send(sock, charArr, buffLen, 0);
}

void sendString(SOCKET& sock, std::string& str) {
	sendCharArr(sock, str.data());
}

char* receiveCharArr(SOCKET& sock, char*& charArr) {
	int charArrLen = 0;
	recv(sock, (char*)&charArrLen, sizeof(charArrLen), 0);
	charArr = new char[charArrLen + 1];
	recv(sock, charArr, charArrLen, 0);
	charArr[charArrLen] = '\0';
	return charArr;
}

std::string receiveString(SOCKET& sock) {
	char* tmpCharArr;
	receiveCharArr(sock, tmpCharArr);
	std::string str = tmpCharArr;
	return str;
}
void sendCommonData(SOCKET& sock, CommonData& data) {
	//Send Type
	sendString(sock, data.type);
	//Send From
	sendString(sock, data.from);
	//Send To
	sendString(sock, data.to);
	//Send message
	sendString(sock, data.message);
}

CommonData receiveCommonData(SOCKET& sock) {
	CommonData data;
	//Receive Type
	data.type = receiveString(sock);
	//Receive From
	data.from = receiveString(sock);
	//Receive To
	data.to = receiveString(sock);
	//Receive message
	data.message = receiveString(sock);
	return data;
}
std::string convertCStrToStr(CString cStr) {
	CT2CA buffer(cStr, CP_UTF8);
	return std::string(buffer);
}