#define USE_NEW_STRLEN // The happy C String Man has implementations of standard C++ library functions if you don't want to include them
#include "c_str_man.hpp" // Include the C String Man
#include <stdio.h>
#include <iostream>

int main(){
    std::cout << substring("Hello, dolly", 0, 5);
    return 0;
}
