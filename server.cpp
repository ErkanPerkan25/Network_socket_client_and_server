/*****************************************************
* Author: Eric Hansson
* File: server.cpp
* Date: 04/20/2024 
* Purpose:
******************************************************/
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

using namespace std;

#define MAX_WAITING 25

int accumulator = 0;

//
int do_server(uint16_t port);

// Function for the thread to do
void* do_work(void *);

struct do_work_struct{
    uint32_t with_sock; 
    struct sockaddr_in *from_cli;
};

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
        // accept incoming client
        int acccepted_socket_client; // socket for actual connected client
        struct sockaddr_in from; // holds client address data
        uint32_t from_len; 

        from_len = sizeof(from);

        acccepted_socket_client = accept(listen_sock, (struct sockaddr *)&from, &from_len);

        // process incoming client
        struct do_work_struct *params = new do_work_struct;
        params->with_sock = acccepted_socket_client; 
        params->from_cli = &from; 

        pthread_create(new pthread_t, NULL, do_work, params);

    }

    return 0;
}

void* do_work(void *generic_ptr){
    struct do_work_struct *actual_ptr = (struct do_work_struct *) generic_ptr;

    uint16_t with_sock; 
    struct sockaddr_in *from_cli;
    with_sock = actual_ptr->with_sock;
    from_cli = actual_ptr->from_cli;

    //Read what the client sent to the server
    int n=0; // how many bytes did we just read?
    char recvln[81]; // actual bytes (characters) read

    while( (n = read(with_sock, recvln, 80)) > 0){
        recvln[n] = '\0'; // null terminate returned "string"
        cout << recvln;

        string command(recvln);

        stringstream word(command);
        string temp;

        while (word >> temp) {
            if(temp == "add"){
                command = temp;
                word >> temp;
                accumulator += stoi(temp); 

                string buffer;
                buffer += to_string(accumulator);
                buffer += '\n';

                char *cbuff = (char *) buffer.c_str();
                int needed = buffer.length();

                while (needed > 0) {
                    int n = write(with_sock, cbuff, needed); 
                    needed -= n;
                    cbuff += n;
                }
            }
            else if (temp == "clear") {
                accumulator = 0; 
            }
            else if (temp == "get") {
                string buffer; // the result we are trying to send back to client
                buffer += to_string(accumulator);
                buffer += '\n';
                // Send from server to the client
                char *cbuff = (char *) buffer.c_str(); // network code needs array of bytes (chars)
                int needed = buffer.length();

                while (needed > 0) {
                    int n = write(with_sock, cbuff, needed);
                    needed -= n;
                    cbuff += n;
                }
                
            }
            else {
                cout << "ERROR: Invalid Command!";
                break;
            }
        }

    }

    cout << accumulator << endl; 
    cout << "Processed a connection from " << inet_ntoa(from_cli->sin_addr) << endl;

    // Closes the connection
    close(with_sock);

    return nullptr;
}

