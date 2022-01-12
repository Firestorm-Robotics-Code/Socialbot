// STD includes
#include <thread>

// Socket includes
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <string>

// Only for testing
#include <iostream>

// Processing positions for requests.
#define DETERMINING_HTTP_METHOD 0
#define DETERMINING_HTTP_PATH 1
#define WAIT_FOR_CONTENT_P1 2
#define WAIT_FOR_CONTENT_P2 3
#define WAIT_FOR_CONTENT_P3 4
#define WAIT_FOR_CONTENT_P4 5
#define READ_CONTENT 6

// HTTP methods
#define HTTP_METHOD_GET 0
#define HTTP_METHOD_POST 1
#define HTTP_METHOD_DELETE 2

struct HTTPRequest{
    uint8_t procpos = DETERMINING_HTTP_METHOD; // Processing position.
    uint8_t method;
    std::string url;
    std::string content;

    std::string _currentData;
};

struct Client{
    HTTPRequest currentRequest;
    int sock;
};

struct ClientLink{ // Link in the chain
    Client *payload;
    ClientLink *next = nullptr;
};

struct ThreadSafeClientPool{ // Custom linked-list implementation for thread safety
    ClientLink *first;
    ClientLink *last;
    int size = 0;
    int newsize = 0;

    void bump(Client* client){ // Add a new client
        ClientLink* cl = (ClientLink*) malloc(sizeof(ClientLink));
        if (size == 0){
            first = cl;
            last = cl;
        }
        else{
            last -> next = cl; // Make cl ahead of the last one so far
            last = cl; // Replace last with cl
        }
        cl -> payload = client;
        newsize ++;
    }

    Client* get(unsigned int pos){
        ClientLink* retCL = first;
        for (unsigned int x = 0; x < pos; x ++){
            retCL = retCL -> next;
        }
        return retCL -> payload;
    }

    void remove(unsigned int pos){
        ClientLink* retCL = first;
        for (unsigned int x = 0; x < pos - 1; x ++){ // We want to get the one before it, you'll see why
            retCL = retCL -> next;
        }
        free(retCL -> payload);
        free(retCL -> next);
        retCL -> next = retCL -> next -> next;
    }

    void run_mainthread(){ // Do main thread synchronization tasks.
        if (size < newsize){ // Bump was called in the interim
            size = newsize; // This is a method of synchronization to maintain thread safety.
        }
    }
};

struct HTTPServer{
    int serverSock = -1; // We don't want this to accidentally write to stdin/out if it is not assigned.
    // Yet.
    struct sockaddr_in address;
    socklen_t addrsize = sizeof(address);

    ThreadSafeClientPool clients;

    static void accepter(HTTPServer* self){ // The self thing is a workaround; because thread functions can't be members, you have to pass in `this` the hard way.
        while (true){
            int cliSock = accept(self -> serverSock, (struct sockaddr*)&self -> address, &self -> addrsize);
            Client *client = new Client;
            client -> sock = cliSock;
            fcntl(cliSock, F_SETFL, O_NONBLOCK);
            self -> clients.bump(client);
        }
    }

    static void reader(HTTPServer* self){
        while (true){
            unsigned int curSize = self -> clients.size;
            for (int x = 0; x < curSize; x ++){
                HTTPRequest req = self -> clients.get(x) -> currentRequest;
                int length = 0;
                if (req -> procpos == DETERMINING_HTTP_METHOD){
                    char buffer;
                    for (uint8_t i = 0; i < 7; i ++){
                        length = recv(req -> sock, &buffer, 1, 0); // Buffer, length, flags.
                        if (length != 1){ // Any non-one case
                            break;
                        }
                        if (buffer == ' '){ // It has reached the end if the buffer is space.
                            std::cout << req -> _currentData << std::endl;
                            req -> procpos = DETERMINING_HTTP_PATH;
                            if (req -> _currentData == "GET"){
                                req -> method = HTTP_METHOD_GET;
                            }
                            else if (req -> _currentData == "POST"){
                                req -> method = HTTP_METHOD_POST;
                            }
                            else if (req -> _currentData == "DELETE"){
                                req -> method = HTTP_METHOD_DELETE;
                            }
                            else {
                                printf("Ooop! They gave us invalid information. Shall we send an error page?");
                            }
                            req -> _currentData = ""; // Empty it
                            break;
                        }
                        req -> _currentData += buffer;
                    }
                }
                if (req -> procpos == DETERMINING_HTTP_PATH){
                    char buffer;
                    while (true){ // T'aint true, it uses break statements.
                        length = recv(req -> sock, &buffer, 1, 0); // Buffer, length, flags.
                        if (buffer == ' '){
                            req -> url = req -> _currentData;
                            req -> _currentData = ""; // Clear it
                            req -> procpos = DETERMINING_HTTP_VERSION; // It can support any HTTP version, this is just to get past it.
                            std::cout << req -> url << std::endl;
                            break;
                        }
                        if (length != 1){
                            break; // Nothing to do but break and wait.
                        }
                        req -> _currentData += buffer;
                    }
                }
                if (req -> procpos == DETERMINING_HTTP_VERSION){
                    char buffer;
                    length = recv(req -> sock, &buffer, 1, 0); // Buffer, length, flags.

                }
                if (req -> procpos == READ_CONTENT){
                    char buffer[1024];
                    length = recv(req -> sock, buffer, 1024, 0);
                    req -> content += buffer;
                    if (length < 1024){ // That's all, folks!
                        req -> procpos = DETERMINING_HTTP_METHOD;
                        std::cout << req -> content;
                    }
                }
            }
        }
    }

    HTTPServer(unsigned int port){
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(port); // They allow 5800-5810 for our stuff, we'll send everything over the one port. 5801 is just to avoid weird potential bugs.
        address.sin_addr.s_addr = INADDR_ANY;
        bind(serverSock, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
        listen(serverSock, 25); // Listen for maximum 25 peoples

        std::thread accepterThread(accepter, this);
        accepterThread.detach();
        std::thread readerThread(reader, this);
        readerThread.detach();
    }

    void run(){
        clients.run_mainthread();
    }
};
