/// This is a comment toggle. Remove the asterisk to activate this entire codee, add it back to deactivate.
#include "httpserver.hpp"
#include "site.hpp"

HTTPServer http(5801, &site::respond, &site::disconnect, 3); // They allow 5800-5810 for our stuff, we'll send everything over the one port. 5801 is just to avoid weird potential bugs.

void gratuitouslyClose(int signum){
    std::cout << "Hey, closin' time!" << std::endl;
    http.closeAll();
    exit(signum);
}

int main(){
    site::beginSite();
    signal(SIGINT, gratuitouslyClose);
    while (true){ // Keep it from exiting until we're ready.

    }
}
//*/
