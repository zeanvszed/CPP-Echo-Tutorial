#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1); // ���� �� ����
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; // �ְ� ���� ���ڿ� ���� ũ�� 256

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock�� �ʱ�ȭ�� (2.2 ���� ���) -> �����ð� 0
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ���� (IPv4)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // ���� ��û �� 5�� ���� ����
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // connect�� accept�� ���� ��ȯ������ ������ Ŭ���̾�Ʈ ����
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) { // Ŭ���̾�Ʈ�� �޽����� �޾Ƽ� �״�� �ٽ� ���� (���ѹݺ�)
		int length = recv(clientSocket, received, sizeof(received), 0); // Ŭ���̾�Ʈ�� ���� ���ڿ� ���� ����
		received[length] = NULL; // ������ ���� NULL����־ ����ġ ���� ���� ������ �߶�
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) { // ����ڰ� "[exit]"�̶� ������ ���α׷� ���� !
			send(clientSocket, received, sizeof(received) - 1, 0); 
			cout << "[���� ����]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // Ŭ���̾�Ʈ�� ���� �ļ� �����ϱ� ������ ���� ���ڴ� �����ϰ� ������
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}