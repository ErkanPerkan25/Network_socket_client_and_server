/*****************************************************
* Author: Eric Hansson
* File: client.cpp
* Date: 
* Purpose:
******************************************************/
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define GET 1
#define CLEAR 2
#define ADD 3

using namespace std;

#define MAX_WAITING 25

int do_client(char *server_name);

string get();
int add(uint16_t num);
string clear();

string command;
int port;
int value;

int main(int argc, char *argv[]){
    if (argc!=3){
        cerr << "USAGE:"  << argv[0] << "<ip address> <port number>" << endl;
        return 1;
    }
    port = stoi(argv[2]);

    getline(cin,command);

    return do_client( argv[1] );
}

int do_client(char *server){
    //cout << "Connecting to " << server_name << endl;
   
    // variable that hold return value from various network calls
    int rval = 0;
    
    // holds IP information about our sever
    struct hostent *server_entry;

    // holds socket information for our connection with server
    // technically a "socket", which holds IP(IP layer) and port(TCP info) for both server
    // (foregin end) and client (local end)
    struct sockaddr_in server_info;

    // covert sever "name" to "numeric" IP address
    server_entry = gethostbyname(server);

    if(!server_entry){
        cerr << "gethostbyname failed for: " << server << endl;
        return 1;
    }
    
    // create a actual network socket
    // AF_INET --> IPv4
    // SOCK_STREAM --> TCP
    // 0 --> use default "flags"
    int my_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Information about the server
    server_info.sin_family = AF_INET;
    server_info.sin_addr = * ( (struct in_addr *)server_entry -> h_addr_list[0] );
    server_info.sin_port = htons(port);

    rval = connect(my_sock, (struct sockaddr *) &server_info, sizeof(server_info));

    if (rval<0) {
        cerr << "connect() failed."  << endl;
        return 1;
    }

    // if we get here we are connected to the server !!!
    
    // Send the command to the server
    string buffer; // the result to the server
    buffer += "From client: ";
    buffer += command;
    buffer += '\n';

    char *cbuff = (char *) buffer.c_str(); // network code needs array of bytes (chars)
    
    int needed = buffer.length();

    while (needed > 0) {
        int n = write(my_sock, cbuff, needed);
        needed -= n;
        cbuff += n;
    }


  
    // Reading response from the server
    int n=0; // how many bytes did we just read?
    char recvln[81]; // actual bytes (characters) read

    while( (n = read(my_sock, recvln, 80)) > 0){
        recvln[n] = '\0'; // null terminate returned "string"
        cout << recvln;
    }

    cout << endl;

    // end network connection
    close(my_sock);

    return 0;
}
