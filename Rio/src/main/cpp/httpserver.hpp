// STD includes
#include <thread>

// Socket includes
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Client{
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
        size ++;
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
        free(retCL -> next);
        retCL -> next = retCL -> next -> next;
    }
};

struct WebServerRobot : ModularRobot{
    int serverSock = -1; // We don't want this to accidentally write to stdin/out if it is not assigned.
    // Yet.
    struct sockaddr_in address;

    ThreadSafeClientPool clients;

    static void accepter(WebServerRobot* self){
        while (true){
            int cli = accept(self -> serverSock);
        }
    }

    void StartCompetition(){
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(5801); // They allow 5800-5810 for our stuff, we'll send everything over the one port. 5801 is just to avoid weird potential bugs.
        address.sin_addr.s_addr = INADDR_ANY;
        bind(serverSock, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
        listen(serverSock, 25); // Listen for maximum 25 peoples
        std::thread accepterThread(accepter);
        accepterThread.detach();
    }
};
