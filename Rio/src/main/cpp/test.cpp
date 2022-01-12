#include "httpserver.hpp"

int main(){
    HTTPServer http(5801);
    while (1){
        http.run();
    }
}
