
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

using namespace std;

string readFile(const string& filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return "";

    return string(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
}

bool sendAll(int client, const string& data)
{
    size_t sent = 0;

    while (sent < data.size())
    {
        ssize_t n = send(
            client, data.data() + sent,
            data.size() - sent, 0
        );

        if (n <= 0) return false;
        sent += n;
    }

    return true;
}

int main()
{
    signal(SIGPIPE, SIG_IGN);

    const char* envPort = getenv("PORT");
    int port = envPort ? atoi(envPort) : 8080;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        cerr << "Socket creation failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&address,
             sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) < 0)
    {
        perror("Listen failed");
        close(serverSocket);
        return 1;
    }

    cout << "Smart Service Queue Server running on port "
         << port << endl;

    while (true)
    {
        int client = accept(serverSocket, nullptr, nullptr);
        if (client < 0) continue;

        char buffer[4096]{};
        ssize_t received = recv(
            client, buffer, sizeof(buffer) - 1, 0
        );

        if (received <= 0)
        {
            close(client);
            continue;
        }

        istringstream requestStream(
            string(buffer, received)
        );

        string method, path, version;
        requestStream >> method >> path >> version;

        size_t query = path.find('?');
        if (query != string::npos)
            path = path.substr(0, query);

        string filename, contentType;

        if (method == "GET" &&
            (path == "/" || path == "/index.html"))
        {
            filename = "index.html";
            contentType = "text/html; charset=UTF-8";
        }
        else if (method == "GET" && path == "/style.css")
        {
            filename = "style.css";
            contentType = "text/css; charset=UTF-8";
        }
        else if (method == "GET" && path == "/script.js")
        {
            filename = "script.js";
            contentType = "application/javascript; charset=UTF-8";
        }

        string body, status;

        if (filename.empty())
        {
            status = "404 Not Found";
            body = "404 Not Found";
            contentType = "text/plain; charset=UTF-8";
        }
        else
        {
            ifstream check(filename, ios::binary);
            if (!check.is_open())
            {
                status = "404 Not Found";
                body = "File not found: " + filename;
                contentType = "text/plain; charset=UTF-8";
            }
            else
            {
                check.close();
                body = readFile(filename);
                status = "200 OK";
            }
        }

        string response =
            "HTTP/1.1 " + status + "\r\n" +
            "Content-Type: " + contentType + "\r\n" +
            "Content-Length: " + to_string(body.size()) + "\r\n" +
            "Connection: close\r\n\r\n" + body;

        sendAll(client, response);

        cout << method << " " << path
             << " -> " << status << endl;

        close(client);
    }

    close(serverSocket);
    return 0;
}
