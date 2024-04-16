/*****************************************************
* Author: Eric Hansson
* File: server.cpp
* Date: 
* Purpose:
******************************************************/
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define MAX_WAITING 25

uint16_t accumulator = 0;

string get();
string add();
string clear();

int main(int argc, char *argv[]){
    if (argc!=2) {
        cerr << "USAGE:"  << argv[0] << "<port number>" << endl;
        return 1;
    }

    return 0;
}

