#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // For inet_pton

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Use the same port number as the server

    // Convert the IP address to binary form and store it in serverAddress.sin_addr
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address: 127.0.0.1" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server!" << std::endl;
    std::cout << "Enter your message (type 'exit' to quit):" << std::endl;

    std::string message;
    while (true) {
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        // Send the message to the server
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // Close the socket and clean up WinSock resources
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}