/*****************************************************
* Author: Eric Hansson
* File: server.cpp
* Date: 
* Purpose:
******************************************************/
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define GET 1
#define CLEAR 2
#define ADD 3

using namespace std;

#define MAX_WAITING 25

uint16_t accumulator = 0;

//
int do_server(uint16_t port);

// Function for the thread to do
void* do_work(void *);

struct do_work_struct{
    uint32_t with_sock; 
    struct sockaddr_in *from_cli;
};

string get(uint16_t value){
    return to_string(value) += '\n';
}
string add();
string clear();

int main(int argc, char *argv[]){
    if (argc!=2) {
        cerr << "USAGE:"  << argv[0] << "<port number>" << endl;
        return 1;
    }

    return do_server( stoi(argv[1]) );
}

int do_server(uint16_t port){
    int listen_sock;

    // local address in the socket
    struct sockaddr_in local_addr;

    // create socket, listen for connection
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    // failed to create socket, print error message
    if(listen_sock < 0){
        cerr << "could not create socket" << endl;
        return 1;
    }

    local_addr.sin_family = AF_INET; // use IPv4
    local_addr.sin_addr.s_addr = INADDR_ANY; // use "wildcard" IP address
    local_addr.sin_port = htons(port); // port num

    // binds to the socket?
    int rval = bind(listen_sock, (struct sockaddr *) &local_addr, sizeof(local_addr));

    // If the connection fails, print error message
    if (rval != 0) {
        cerr << "Binding failed - possible cause include: "  << endl
            << "    * invalid port numer (access denied or already in use?)" << endl
            << "    * invalid local address (did you use the wildcard?)" << endl;
        return 1;
    }

    rval = listen(listen_sock, MAX_WAITING);

    if(rval!=0){
        cerr << "listen() failed!" << endl;
        return 1;
    }

    while (true) {
        int acccepted_socket_client;
        struct sockaddr_in from;
        uint32_t from_len; 

        from_len = sizeof(from);

        acccepted_socket_client = accept(listen_sock, (struct sockaddr *)&from, &from_len);

        struct do_work_struct *params = new do_work_struct;
        params->with_sock = acccepted_socket_client; 
        params->from_cli = &from; 

        pthread_create(new pthread_t, NULL, do_work, (void *)params);
    } 

    return 0;
}

void* do_work(void *generic_ptr){
    struct do_work_struct *actual_ptr = (struct do_work_struct *) generic_ptr;

    uint16_t with_sock; 
    struct sockaddr_in *from_cli;
    with_sock = actual_ptr->with_sock;
    from_cli = actual_ptr->from_cli;
    
    /*
    string buffer; // the result we are trying to send back to client
    buffer = "Your IP is ";
    buffer += inet_ntoa(from_cli->sin_addr);
    buffer += "\n";
    */

    //Read what the client sent to the server
    int n=0; // how many bytes did we just read?
    char recvln[81]; // actual bytes (characters) read

    while( (n = read(with_sock, recvln, 80)) > 0){
        recvln[n] = '\0'; // null terminate returned "string"
        cout << recvln;
    }
    cout << endl;

    // Send from server to the client
    /*
    char *cbuff = (char *) buffer.c_str(); // network code needs array of bytes (chars)
    
    int needed = buffer.length();

    while (needed > 0) {
        int n = write(with_sock, cbuff, needed);
        needed -= n;
        cbuff += n;
    }
    */
    
    cout << "Processed a connection from " << inet_ntoa(from_cli->sin_addr) << endl;

    return nullptr;
}

