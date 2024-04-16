/*****************************************************
* Author: Eric Hansson
* File: client.cpp
* Date: 
* Purpose:
******************************************************/
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define MAX_WAITING 25

string get();
int add(uint16_t num);
string clear();

int main(int argc, char *argv[]){
    if (argc!=2 && argc!=3) {
        cerr << "USAGE:"  << argv[0] << "<ip address> <port number>" << endl;
        return 1;
    }

    return 0;
}

