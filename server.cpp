#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string readHTML()
{
    ifstream file("index.html");

    if (!file.is_open())
    {
        return
            "<html>"
            "<body>"
            "<h1>Error</h1>"
            "<p>index.html file not found!</p>"
            "</body>"
            "</html>";
    }

    string html;
    string line;

    while (getline(file, line))
    {
        html += line;
        html += "\n";
    }

    file.close();

    return html;
}


int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Winsock initialization failed!" << endl;
        return 1;
    }


    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );


    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }


    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr =
        inet_addr("127.0.0.1");

    serverAddress.sin_port =
        htons(8080);


    if (bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) == SOCKET_ERROR)
    {
        cout << "Bind failed! Error: "
             << WSAGetLastError()
             << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }


    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        cout << "Listen failed!" << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }


    cout << "====================================" << endl;
    cout << " SMART SERVICE QUEUE SERVER" << endl;
    cout << "====================================" << endl;

    cout << "Server started successfully!" << endl;

    cout << "Open:" << endl;
    cout << "http://127.0.0.1:8080" << endl;

    cout << "Waiting for browser..." << endl;


    while (true)
    {
        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );


        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Client connection failed!"
                 << endl;

            continue;
        }


        cout << "Client connected!" << endl;


        char buffer[4096];


        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );


        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';

            cout << "Request received."
                 << endl;
        }


        // Read actual index.html

        string html = readHTML();


        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: "
            + to_string(html.length())
            + "\r\n"
            "Connection: close\r\n"
            "\r\n"
            + html;


        send(
            clientSocket,
            response.c_str(),
            static_cast<int>(response.length()),
            0
        );


        cout << "index.html sent successfully."
             << endl;


        closesocket(clientSocket);


        cout << "Waiting for next client..."
             << endl;
    }


    closesocket(serverSocket);

    WSACleanup();

    return 0;
}