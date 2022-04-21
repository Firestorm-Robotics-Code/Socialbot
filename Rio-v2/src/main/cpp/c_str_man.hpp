/// Comment toggle
// The happy C String Man
// Has a lot of c-string utilities
// You know, because c-strings are big pain
// The file is in lowercase because its a utility which should be copied to all of my programs that need it
#ifndef CSTRINGMAN
#define CSTRINGMAN
#include <stdio.h>
#include <iostream>
#include <cstdarg>
#include <cstring>

#ifdef USE_NEW_STRLEN
long unsigned int strlen(const char *array){ // I don't like void* because of ambiguity but this will work for any c string - const or not
    unsigned int length;
    for (length = 0; (char)array[length]!='\0';length++);
    return length;
}
#endif

const char* substring(const char* string, int start, int end){ // Get characters start-end inclusive from string
    char* buffer = (char*)malloc(end-start);
    for (int i = 0; i < end - start; i++){
        buffer[i] = string[start + i];
    }
    return (const char*)buffer;
}

const char* inttostring(long data){ // A wrapper around sprintf designed for inline functions
    char* buffer = (char*)malloc(21);
    sprintf(buffer, "%ld",data);
    return (const char*)buffer;
}

void floattostring(float data, char* buffer){
    sprintf(buffer,"%f",data);
}

const char* constchar_concat(const char* part1, const char* part2){
    char* helloDolly = (char*)malloc(std::strlen(part2)+std::strlen(part1)); // Offset it so one of the null characters is erased
    for (int i = 0; i < std::strlen(part1); i++){ // Copy part2 with offset into helloDolly;
        helloDolly[i] = part1[i];
    }
    int startpos = std::strlen(part1); // Erase the null character
    for (int i = 0; i < std::strlen(part2); i++){ // Copy part2 with offset into helloDolly;
        helloDolly[startpos+i] = part2[i];
    }
    return (const char*)helloDolly;
}

const char* constchar_concat_many(int count, ...){
    va_list list;
    va_start(list, count);
    unsigned long length = 0;
    for (short i = 0; i < count; i++){
        length += std::strlen(va_arg(list, const char*));
    }
    char* buffer = (char*)malloc(length + 1); // Needs a null character at the end!
    buffer[length] ='\0'; // And here, my fwends, is de nuww ckawacter
    unsigned long startpos = 0;
    va_start(list, count);
    for (short i = 0; i < count; i++){
        const char* data = va_arg(list, const char*);
        long unsigned int length = std::strlen(data);
        for (int i = 0; i < length; i++){
            buffer[i + startpos] = data[i];
        }
        startpos += length;
    }
    return (const char*)buffer;
}
#endif
//*/
#ifndef MESSAGE
#define MESSAGE
const char* message ="Hello. What are you doing here? Why are you here? I put this in here as a last message from me to the team. It's very well hidden, and I won't tell you what file it's in so you won't find it. I want you to know something very important, however: the administration code to my web console. The code is my birthday: july18_07. You're an idiot if you couldn't find it just by reading the source code, it's literally a macro.";
#endif
