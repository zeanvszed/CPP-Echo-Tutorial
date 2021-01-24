#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket; // 클라이언트 측이 사용하는 소켓
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; // 서버와 동일한 포트 사용
	char received[256];
	string sent; // 사용자 입력 문자열 (보낼)

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock 초기화
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열 IP를 네트워크 바이트 형식으로 -> 여기서 쓴 문자열은 자기자신의 컴퓨터 IP주소 의미하는 약속된 문자열
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수를 네트워크 바이트 형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	
	cout << "[현재 상태] connect\n";

	while (1) { // 반복적으로 서버로 메시지 전송하고 에코 메시지 전달 받음 !
		cout << "[메시지 전송]: ";
		getline(cin, sent); // 보낼 문자열 입력받기
		if (sent == "") continue; // 사용자가 엔터만 치면 무시하고 계속 진행
		send(clientSocket, sent.c_str(), sent.length(), 0); // c_str() 통해 문자열 버퍼 형태로 바꾸어줌
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0'; // 마지막 NULL값
		if (strcmp(received, "[exit]") == 0) { // 서버에서 돌아온게 exit일때 종료
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}