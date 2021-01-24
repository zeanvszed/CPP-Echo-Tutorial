#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket; // Ŭ���̾�Ʈ ���� ����ϴ� ����
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; // ������ ������ ��Ʈ ���
	char received[256];
	string sent; // ����� �Է� ���ڿ� (����)

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ���ڿ� IP�� ��Ʈ��ũ ����Ʈ �������� -> ���⼭ �� ���ڿ��� �ڱ��ڽ��� ��ǻ�� IP�ּ� �ǹ��ϴ� ��ӵ� ���ڿ�
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	
	cout << "[���� ����] connect\n";

	while (1) { // �ݺ������� ������ �޽��� �����ϰ� ���� �޽��� ���� ���� !
		cout << "[�޽��� ����]: ";
		getline(cin, sent); // ���� ���ڿ� �Է¹ޱ�
		if (sent == "") continue; // ����ڰ� ���͸� ġ�� �����ϰ� ��� ����
		send(clientSocket, sent.c_str(), sent.length(), 0); // c_str() ���� ���ڿ� ���� ���·� �ٲپ���
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0'; // ������ NULL��
		if (strcmp(received, "[exit]") == 0) { // �������� ���ƿ°� exit�϶� ����
			cout << "[���� ����]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}