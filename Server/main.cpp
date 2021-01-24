#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1); // 오류 시 종료
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; // 주고 받을 문자열 버퍼 크기 256

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을 초기화함 (2.2 버젼 사용) -> 성공시가 0
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성 (IPv4)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수를 네트워크 바이트 형식으로
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // 동시 요청 총 5명 까지 가능
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // connect의 accept에 대한 반환값으로 생성된 클라이언트 소켓
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) { // 클라이언트의 메시지를 받아서 그대로 다시 전달 (무한반복)
		int length = recv(clientSocket, received, sizeof(received), 0); // 클라이언트가 보낸 문자열 버퍼 길이
		received[length] = NULL; // 마지막 문자 NULL집어넣어서 예기치 못한 문자 왔을때 잘라냄
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) { // 사용자가 "[exit]"이라 보내면 프로그램 종료 !
			send(clientSocket, received, sizeof(received) - 1, 0); 
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // 클라이언트가 엔터 쳐서 보내니까 마지막 공백 문자는 제거하고 보낸다
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}